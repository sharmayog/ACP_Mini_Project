# ACP_Mini_Project
# 🎨 2D Graphics Editor in C

A console-based **2D Graphics Editor** implemented in **C** that allows users to draw and manage geometric shapes on a character-based canvas using symbols such as `*` and `_`.

The application stores shapes in memory and redraws the entire canvas whenever changes are made, ensuring consistency when shapes are modified or deleted.

---

## 📌 Features

* Draw geometric shapes:

  * Circle
  * Rectangle
  * Line
  * Triangle
* Display shapes on a 2D ASCII canvas
* Modify existing shapes
* Delete shapes
* List all stored shapes
* Custom drawing symbols
* Automatic canvas re-rendering after updates

---

## 🖥️ Canvas Specifications

| Property             | Value       |
| -------------------- | ----------- |
| Width                | 60 Columns  |
| Height               | 25 Rows     |
| Maximum Shapes       | 50          |
| Background Character | Space (` `) |

```c
#define ROWS 25
#define COLS 60
#define MAX_SHAPES 50
#define EMPTY ' '
```

---

## 📂 Shape Types

### 1. Circle

Stores:

* Center `(x1, y1)`
* Radius

### 2. Rectangle

Stores:

* Top-left corner `(x1, y1)`
* Bottom-right corner `(x2, y2)`

### 3. Line

Stores:

* Start point `(x1, y1)`
* End point `(x2, y2)`

### 4. Triangle

Stores:

* Three vertices:

  * `(x1, y1)`
  * `(x2, y2)`
  * `(x3, y3)`

---

## ⚙️ Algorithms Used

### Bresenham's Line Algorithm

Used for drawing straight lines efficiently.

### Midpoint Circle Algorithm

Used for rendering circle outlines.

### Rectangle Drawing

Constructed using four line segments.

### Triangle Drawing

Constructed using three connected line segments.

---

## 🏗️ Project Structure

```text
.
├── graphics_editor.c
└── README.md
```

---

## 🚀 Compilation

Compile using GCC:

```bash
gcc graphics_editor.c -o graphics_editor
```

Run the program:

```bash
./graphics_editor
```

---

## 📋 Main Menu

```text
----------- MENU -----------
1. Add a shape
2. Delete a shape
3. Modify a shape
4. List all shapes
5. Display picture
0. Exit
-----------------------------
```

---

## ➕ Adding Shapes

Choose a shape type and enter the required coordinates.

Example:

```text
-- Add Shape --
1. Circle
2. Rectangle
3. Line
4. Triangle

Choose shape type: 2
Top-left x: 5
Top-left y: 3
Bottom-right x: 20
Bottom-right y: 10
Enter symbol: *
```

---

## 🖼️ Sample Output

```text
+------------------------------------------------------------+
|                                                            |
|                                                            |
|     ****************                                       |
|     *              *                                       |
|     *              *                                       |
|     *              *                                       |
|     *              *                                       |
|     ****************                                       |
|                                                            |
+------------------------------------------------------------+
```

---

## 🔄 Rendering Strategy

Instead of directly erasing pixels from the canvas, the program stores all shapes in a shape list.

Whenever a shape is:

* Added
* Modified
* Deleted

the canvas is:

1. Cleared
2. Rebuilt from scratch
3. Every active shape is redrawn in order

This approach guarantees consistent rendering and prevents graphical artifacts.

---

## 📚 Concepts Demonstrated

* Structures (`struct`)
* Enumerations (`enum`)
* Arrays
* Function Modularization
* Input Validation
* Computer Graphics Fundamentals
* Bresenham's Algorithm
* Midpoint Circle Algorithm

---

## 🔮 Future Enhancements

* Filled Shapes
* Shape Colors
* Save/Load Drawings
* Undo/Redo Support
* Shape Layering
* Mouse Interaction
* Export to Image Formats
* Dynamic Canvas Resizing

---

## 🎯 Learning Objectives

This project is ideal for learning:

* C Programming
* Data Structures
* Graphics Algorithms
* Coordinate Systems
* Modular Software Design

---

## 👨‍💻 Author

Developed as a console-based graphics editor project in C for educational and learning purposes.

### ⭐ If you found this project useful, consider giving the repository a star!
