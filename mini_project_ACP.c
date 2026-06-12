/* ============================================================
   2D GRAPHICS EDITOR (using '*' and '_')
   ------------------------------------------------------------
   - The picture is stored in a 2D array of characters (canvas).
   - Shapes (circle, rectangle, line, triangle) are stored in a
     list. Each shape remembers its own coordinates and symbol.
   - Whenever the picture changes (add / delete / modify), the
     canvas is rebuilt from scratch by re-drawing every shape in
     the list, in order. This keeps deletes/edits consistent.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>

#define ROWS        25     /* height of the canvas (number of rows)    */
#define COLS        60     /* width of the canvas  (number of columns) */
#define MAX_SHAPES  50      /* maximum number of shapes that can be stored */
#define EMPTY       ' '     /* background character                     */

/* ---------------- Shape types ---------------- */
typedef enum {
    CIRCLE = 1,
    RECTANGLE,
    LINE,
    TRIANGLE
} ShapeType;

/* ---------------- Shape structure ----------------
   Different shapes use different fields:
     CIRCLE    -> (x1,y1) = center,            radius = radius
     RECTANGLE -> (x1,y1) = top-left corner,    (x2,y2) = bottom-right corner
     LINE      -> (x1,y1) -> (x2,y2)
     TRIANGLE  -> (x1,y1), (x2,y2), (x3,y3) = the three vertices
     symbol    -> character used to draw the shape ('*' or '_' recommended)
     active    -> 1 if this slot holds a real shape, 0 if empty/deleted
*/
typedef struct {
    ShapeType type;
    int  x1, y1;
    int  x2, y2;
    int  x3, y3;
    int  radius;
    char symbol;
    int  active;
} Shape;

Shape shapes[MAX_SHAPES];
int   shapeCount = 0;

char canvas[ROWS][COLS];

/* ---------------- Function prototypes ---------------- */
void initCanvas(void);
void plot(int row, int col, char ch);

void drawLineShape(int x1, int y1, int x2, int y2, char ch);
void drawRectangleShape(int x1, int y1, int x2, int y2, char ch);
void drawCircleShape(int cx, int cy, int r, char ch);
void drawTriangleShape(int x1, int y1, int x2, int y2, int x3, int y3, char ch);

void renderCanvas(void);
void displayCanvas(void);

void addShape(void);
void deleteShape(void);
void modifyShape(void);
void listShapes(void);

int  readInt(const char *prompt, int min, int max);
char readSymbol(void);
void clearInputBuffer(void);


/* ============================================================
   MAIN
   ============================================================ */
int main(void) {
    int choice;

    printf("=========================================\n");
    printf("   2D GRAPHICS EDITOR (using '*' and '_')\n");
    printf("   Canvas size : %d columns x %d rows\n", COLS, ROWS);
    printf("   Valid coords: x = 0..%d , y = 0..%d\n", COLS - 1, ROWS - 1);
    printf("=========================================\n");

    do {
        printf("\n----------- MENU -----------\n");
        printf("1. Add a shape\n");
        printf("2. Delete a shape\n");
        printf("3. Modify a shape\n");
        printf("4. List all shapes\n");
        printf("5. Display picture\n");
        printf("0. Exit\n");
        printf("-----------------------------\n");

        choice = readInt("Enter your choice: ", 0, 5);

        switch (choice) {
            case 1: addShape();    break;
            case 2: deleteShape(); break;
            case 3: modifyShape(); break;
            case 4: listShapes();  break;
            case 5:
                renderCanvas();
                displayCanvas();
                break;
            case 0:
                printf("Exiting editor. Goodbye!\n");
                break;
        }
    } while (choice != 0);

    return 0;
}


/* ============================================================
   INPUT HELPERS
   ============================================================ */

/* Discards everything left in the input buffer up to and
   including the next newline. Prevents an infinite loop if the
   user types something that is not a number. */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

/* Reads an integer, re-prompting until the value lies within
   [min, max] (inclusive). */
int readInt(const char *prompt, int min, int max) {
    int value;
    int ok;

    while (1) {
        printf("%s", prompt);
        ok = scanf("%d", &value);
        clearInputBuffer();

        if (ok == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter a number between %d and %d.\n", min, max);
    }
}

/* Reads the single character that will be used to draw a shape.
   '*' and '_' are recommended, but any printable, non-space
   character is accepted. */
char readSymbol(void) {
    char ch;
    while (1) {
        printf("Enter symbol to draw with (recommended '*' or '_'): ");
        ch = (char) getchar();
        clearInputBuffer();

        if (ch > 32 && ch < 127) {   /* printable and not a space */
            return ch;
        }
        printf("Invalid symbol. Enter a single printable, non-space character.\n");
    }
}


/* ============================================================
   CANVAS / DRAWING FUNCTIONS
   ============================================================ */

/* Fills the entire canvas with the background character. */
void initCanvas(void) {
    int r, c;
    for (r = 0; r < ROWS; r++) {
        for (c = 0; c < COLS; c++) {
            canvas[r][c] = EMPTY;
        }
    }
}

/* Safely writes one character onto the canvas. Coordinates that
   fall outside the canvas are silently ignored, so shapes that
   are partly off-screen never crash the program. */
void plot(int row, int col, char ch) {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        canvas[row][col] = ch;
    }
}

/* Draws a straight line from (x1,y1) to (x2,y2) using
   Bresenham's line algorithm. x = column, y = row. */
void drawLineShape(int x1, int y1, int x2, int y2, char ch) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 >= x1) ? 1 : -1;
    int sy = (y2 >= y1) ? 1 : -1;
    int err = dx - dy;
    int x = x1, y = y1;
    int e2;

    while (1) {
        plot(y, x, ch);
        if (x == x2 && y == y2) break;

        e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 <  dx) { err += dx; y += sy; }
    }
}

/* Draws an axis-aligned rectangle. (x1,y1) and (x2,y2) are two
   opposite corners and may be given in any order. */
void drawRectangleShape(int x1, int y1, int x2, int y2, char ch) {
    int left   = (x1 < x2) ? x1 : x2;
    int right  = (x1 > x2) ? x1 : x2;
    int top    = (y1 < y2) ? y1 : y2;
    int bottom = (y1 > y2) ? y1 : y2;

    drawLineShape(left,  top,    right, top,    ch); /* top edge    */
    drawLineShape(left,  bottom, right, bottom, ch); /* bottom edge */
    drawLineShape(left,  top,    left,  bottom, ch); /* left edge   */
    drawLineShape(right, top,    right, bottom, ch); /* right edge  */
}

/* Draws a circle outline using the midpoint circle algorithm.
   (cx,cy) is the center and r is the radius. */
void drawCircleShape(int cx, int cy, int r, char ch) {
    int x = r;
    int y = 0;
    int err = 0;

    while (x >= y) {
        plot(cy + y, cx + x, ch);
        plot(cy + x, cx + y, ch);
        plot(cy + x, cx - y, ch);
        plot(cy + y, cx - x, ch);
        plot(cy - y, cx - x, ch);
        plot(cy - x, cx - y, ch);
        plot(cy - x, cx + y, ch);
        plot(cy - y, cx + x, ch);

        y++;
        err += 1 + 2 * y;
        if (2 * (err - x) + 1 > 0) {
            x--;
            err += 1 - 2 * x;
        }
    }
}

/* Draws a triangle by connecting its three vertices with three
   straight lines. */
void drawTriangleShape(int x1, int y1, int x2, int y2, int x3, int y3, char ch) {
    drawLineShape(x1, y1, x2, y2, ch);
    drawLineShape(x2, y2, x3, y3, ch);
    drawLineShape(x3, y3, x1, y1, ch);
}

/* Rebuilds the canvas from scratch using the stored shape list.
   Called before every display so that deletes/edits/overlaps
   are always reflected correctly. */
void renderCanvas(void) {
    int i;
    initCanvas();

    for (i = 0; i < shapeCount; i++) {
        if (!shapes[i].active) continue;

        switch (shapes[i].type) {
            case CIRCLE:
                drawCircleShape(shapes[i].x1, shapes[i].y1, shapes[i].radius, shapes[i].symbol);
                break;
            case RECTANGLE:
                drawRectangleShape(shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2, shapes[i].symbol);
                break;
            case LINE:
                drawLineShape(shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2, shapes[i].symbol);
                break;
            case TRIANGLE:
                drawTriangleShape(shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2,
                                   shapes[i].x3, shapes[i].y3, shapes[i].symbol);
                break;
        }
    }
}

/* Prints the canvas with a simple border so the picture area is
   easy to see. */
void displayCanvas(void) {
    int r, c;

    printf("\n");

    printf("+");
    for (c = 0; c < COLS; c++) printf("-");
    printf("+\n");

    for (r = 0; r < ROWS; r++) {
        printf("|");
        for (c = 0; c < COLS; c++) {
            putchar(canvas[r][c]);
        }
        printf("|\n");
    }

    printf("+");
    for (c = 0; c < COLS; c++) printf("-");
    printf("+\n");
}


/* ============================================================
   SHAPE MANAGEMENT: ADD / DELETE / MODIFY / LIST
   ============================================================ */

/* Prompts for the details of a new shape, validates the input,
   and appends it to the shapes[] list. */
void addShape(void) {
    int typeChoice;
    Shape s;
    int maxRadius = (ROWS < COLS) ? ROWS : COLS;

    if (shapeCount >= MAX_SHAPES) {
        printf("Shape limit (%d) reached. Cannot add more shapes.\n", MAX_SHAPES);
        return;
    }

    printf("\n-- Add Shape --\n");
    printf("1. Circle\n2. Rectangle\n3. Line\n4. Triangle\n");
    typeChoice = readInt("Choose shape type: ", 1, 4);

    s.active = 1;

    switch (typeChoice) {
        case 1: /* Circle */
            s.type   = CIRCLE;
            s.x1     = readInt("Center x (0-59): ", 0, COLS - 1);
            s.y1     = readInt("Center y (0-24): ", 0, ROWS - 1);
            s.radius = readInt("Radius (1-25): ", 1, maxRadius);
            s.symbol = readSymbol();
            break;

        case 2: /* Rectangle */
            s.type = RECTANGLE;
            s.x1   = readInt("Top-left x (0-59): ", 0, COLS - 1);
            s.y1   = readInt("Top-left y (0-24): ", 0, ROWS - 1);
            s.x2   = readInt("Bottom-right x (0-59): ", 0, COLS - 1);
            s.y2   = readInt("Bottom-right y (0-24): ", 0, ROWS - 1);
            s.symbol = readSymbol();
            break;

        case 3: /* Line */
            s.type = LINE;
            s.x1   = readInt("Start x (0-59): ", 0, COLS - 1);
            s.y1   = readInt("Start y (0-24): ", 0, ROWS - 1);
            s.x2   = readInt("End x (0-59): ", 0, COLS - 1);
            s.y2   = readInt("End y (0-24): ", 0, ROWS - 1);
            s.symbol = readSymbol();
            break;

        case 4: /* Triangle */
            s.type = TRIANGLE;
            s.x1   = readInt("Vertex 1 x (0-59): ", 0, COLS - 1);
            s.y1   = readInt("Vertex 1 y (0-24): ", 0, ROWS - 1);
            s.x2   = readInt("Vertex 2 x (0-59): ", 0, COLS - 1);
            s.y2   = readInt("Vertex 2 y (0-24): ", 0, ROWS - 1);
            s.x3   = readInt("Vertex 3 x (0-59): ", 0, COLS - 1);
            s.y3   = readInt("Vertex 3 y (0-24): ", 0, ROWS - 1);
            s.symbol = readSymbol();
            break;
    }

    shapes[shapeCount] = s;
    printf("Shape added successfully as shape #%d.\n", shapeCount);
    shapeCount++;
}

/* Prints a numbered list of every shape currently stored, along
   with its parameters and drawing symbol. */
void listShapes(void) {
    int i;

    if (shapeCount == 0) {
        printf("\nNo shapes have been added yet.\n");
        return;
    }

    printf("\n-- Shape List --\n");
    for (i = 0; i < shapeCount; i++) {
        if (!shapes[i].active) continue;

        printf("#%d: ", i);
        switch (shapes[i].type) {
            case CIRCLE:
                printf("Circle    | center=(%d,%d) radius=%d symbol='%c'\n",
                       shapes[i].x1, shapes[i].y1, shapes[i].radius, shapes[i].symbol);
                break;
            case RECTANGLE:
                printf("Rectangle | corners=(%d,%d)-(%d,%d) symbol='%c'\n",
                       shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2, shapes[i].symbol);
                break;
            case LINE:
                printf("Line      | (%d,%d) -> (%d,%d) symbol='%c'\n",
                       shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2, shapes[i].symbol);
                break;
            case TRIANGLE:
                printf("Triangle  | (%d,%d), (%d,%d), (%d,%d) symbol='%c'\n",
                       shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2,
                       shapes[i].x3, shapes[i].y3, shapes[i].symbol);
                break;
        }
    }
}

/* Removes a shape by index. The remaining shapes are shifted
   left so indices stay contiguous (#0, #1, #2, ...). */
void deleteShape(void) {
    int index, i;

    listShapes();
    if (shapeCount == 0) return;

    index = readInt("Enter the index of the shape to delete: ", 0, shapeCount - 1);

    for (i = index; i < shapeCount - 1; i++) {
        shapes[i] = shapes[i + 1];
    }
    shapeCount--;

    printf("Shape #%d deleted.\n", index);
}

/* Lets the user pick an existing shape by index and re-enter all
   of its parameters (the shape's type stays the same). */
void modifyShape(void) {
    int index;
    Shape *s;
    int maxRadius = (ROWS < COLS) ? ROWS : COLS;

    listShapes();
    if (shapeCount == 0) return;

    index = readInt("Enter the index of the shape to modify: ", 0, shapeCount - 1);
    s = &shapes[index];

    printf("\n-- Modify Shape #%d --\n", index);

    switch (s->type) {
        case CIRCLE:
            printf("Editing Circle (enter new values)\n");
            s->x1     = readInt("New center x (0-59): ", 0, COLS - 1);
            s->y1     = readInt("New center y (0-24): ", 0, ROWS - 1);
            s->radius = readInt("New radius (1-25): ", 1, maxRadius);
            s->symbol = readSymbol();
            break;

        case RECTANGLE:
            printf("Editing Rectangle (enter new values)\n");
            s->x1 = readInt("New top-left x (0-59): ", 0, COLS - 1);
            s->y1 = readInt("New top-left y (0-24): ", 0, ROWS - 1);
            s->x2 = readInt("New bottom-right x (0-59): ", 0, COLS - 1);
            s->y2 = readInt("New bottom-right y (0-24): ", 0, ROWS - 1);
            s->symbol = readSymbol();
            break;

        case LINE:
            printf("Editing Line (enter new values)\n");
            s->x1 = readInt("New start x (0-59): ", 0, COLS - 1);
            s->y1 = readInt("New start y (0-24): ", 0, ROWS - 1);
            s->x2 = readInt("New end x (0-59): ", 0, COLS - 1);
            s->y2 = readInt("New end y (0-24): ", 0, ROWS - 1);
            s->symbol = readSymbol();
            break;

        case TRIANGLE:
            printf("Editing Triangle (enter new values)\n");
            s->x1 = readInt("New vertex 1 x (0-59): ", 0, COLS - 1);
            s->y1 = readInt("New vertex 1 y (0-24): ", 0, ROWS - 1);
            s->x2 = readInt("New vertex 2 x (0-59): ", 0, COLS - 1);
            s->y2 = readInt("New vertex 2 y (0-24): ", 0, ROWS - 1);
            s->x3 = readInt("New vertex 3 x (0-59): ", 0, COLS - 1);
            s->y3 = readInt("New vertex 3 y (0-24): ", 0, ROWS - 1);
            s->symbol = readSymbol();
            break;
    }

    printf("Shape #%d updated.\n", index);
}