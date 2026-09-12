# ABC — A Chess Programming Book

**ABC (Array Based Chess)** is a small, didactic chess engine written in C — but this repository is **a book repository first, and an engine repository second**.

The main purpose of this project is to document how a complete chess engine is built, piece by piece, in a way that is understandable to someone learning chess programming.

The source code is there because the book is about a real, working engine. The engine is the subject of the book — **the book is the project**.

## The Book

The repository contains a complete static HTML book explaining the implementation of ABC.

The book starts with the basic foundations of a chess engine and gradually builds toward a complete UCI-compatible program:

* project structure
* C and GCC setup
* board representation
* 0x88
* piece encoding
* move encoding
* move generation
* attacked-square detection
* move making and legality
* position restoration
* perft testing
* position hashing
* evaluation
* move ordering
* repetition detection
* quiescence search
* negamax
* alpha-beta search
* iterative deepening
* UCI communication
* time management

The goal is not to present the most sophisticated or fastest implementation possible.

The goal is to **understand the code**.

## This Is a Book Repository

It is worth making the distinction explicit:

> **This is not an engine repository with a book attached to it. It is a book repository with an engine covered by the book.**

ABC deliberately favors clarity, simplicity, and experimentation over maximum playing strength.

Some implementations could certainly be made faster or more sophisticated. Positions could be restored incrementally instead of copied. Hashes could be updated incrementally. Move ordering could use more advanced techniques. The evaluation could be considerably stronger. Search could contain many more pruning techniques.

Those are useful directions for experimentation — but they are not the primary purpose of ABC.

The code is intentionally written so that a reader can open a function and understand what it is doing.

### `index.html` and `book/`

The actual book.

The HTML pages contain the explanations accompanying the ABC source code, from the first concepts through the complete engine.

### C source code

The source code is the implementation being documented throughout the book.

You can compile it, run it, modify it, break it, debug it, and use it as a starting point for your own experiments.

### Example games

The repository also contains example games demonstrating ABC in action, including games where **ABC defeats TSCP**.

These games are not meant to prove that ABC is a strong chess engine. They are simply examples showing that the small engine described in the book is capable of playing complete games and finding interesting results.

## Why ABC?

Chess programming can be intimidating when you encounter it as a collection of disconnected concepts.

You learn about:

* 0x88
* move generation
* perft
* alpha-beta
* quiescence search
* evaluation
* UCI
* hashing

…but knowing what each term means is different from understanding how all of them fit together into one working program.

ABC is an attempt to bridge that gap.

Instead of presenting isolated algorithms, the book follows one complete engine and explains how its parts connect.

A generated move becomes an encoded integer.

That move can be played on the board.

The resulting position can be checked for legality.

The position can be searched.

The position can be evaluated.

The result can be converted back into UCI notation.

And eventually the engine can communicate that move to a chess GUI.

That complete path is what makes the project useful.

## Learning Philosophy

ABC is intentionally **not optimized for educational minimalism at the expense of completeness**, nor is it optimized purely for playing strength.

It tries to occupy the useful middle ground:

**small enough to understand, complete enough to play chess.**

The code is deliberately straightforward in places where a production engine might use significantly more complicated techniques.

That makes ABC a good place to:

* learn how chess engines work
* experiment with search algorithms
* debug chess programming concepts
* study a complete C chess engine
* build your own engine from an understandable foundation
* replace individual components and see what happens

## Building ABC

ABC is written in C and can be compiled with GCC.

For example:

```bash
gcc -Ofast -flto *.c -o abc.exe
```

or just

```bash
make
```

The resulting executable communicates using the **UCI (Universal Chess Interface)** protocol and can therefore be connected to a compatible chess GUI.

The book explains the build environment and the engine's UCI interface in more detail.

## A Starting Point, Not the Final Word

ABC is intentionally modest.

Once you understand the existing implementation, there is a huge amount of room for experimentation.

You could try adding:

* transposition tables
* stronger evaluation
* better move ordering
* principal variation search
* late move reductions
* null-move pruning
* aspiration windows
* opening books
* endgame knowledge
* NNUE
* bitboards
* parallel search
* improved time management

The important part is that after reading the book, these techniques have somewhere to go.

You are no longer trying to understand them in isolation. You have a complete engine in front of you and can experiment with it directly.

## About the Author

ABC grew out of many years of experimenting with chess programming.

One of the motivations for creating this book was simple:

**I wish I had a book like this when I started chess programming nearly ten years ago.**

Chess programming takes patience. It took years of experimenting and debugging to reach the point where the fundamentals became intuitive. For example, it took three years back in the day to write a first bug-free move generator capable of passing a complete perft test.

That experience is part of why ABC focuses so strongly on understanding the implementation rather than simply presenting finished algorithms.

## Support

The book is intentionally free because I strongly believe that **knowledge and code should be freely available**.

At the same time, creating and maintaining a project like this takes a considerable amount of time.

If the book helps you learn something, saves you time, inspires you to write your own chess engine, or you simply enjoy the project, donations are very much appreciated.

**PayPal:** `maksymkorzh@gmail.com`

Support makes it easier to continue spending time on projects like this.

## License

ABC is released under the **GNU General Public License v3.0 (GPLv3)**.

The choice is intentional.

I believe that **knowledge and code should remain free**. If someone takes ABC, improves it, modifies it, or builds upon it and distributes the resulting software, the source code should remain available to the people who receive it.

The GPL helps preserve that freedom.

You are free to:

* use ABC for any purpose
* study how it works
* modify the source code
* distribute copies
* distribute modified versions
* build upon the project

Under the terms of the GPLv3, distributed modified versions must also provide the corresponding source code and remain under the same license.

In other words:

> **You are free to build on ABC, but you cannot take the code, close it up, and hide the improvements from the people who receive it.**

The complete license text is available in the [`LICENSE`](LICENSE) file.

### GNU GPLv3

```text
ABC — Array Based Chess

Copyright (C) 2026 Code Monkey King

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
```

The **book itself** can have a separate documentation license if you want—for example, CC BY-SA 4.0—while the **engine source code remains GPLv3**. That would be a particularly good fit for this project: free code, free documentation, and improvements staying free.

## Final Words

ABC is not intended to be the strongest chess engine.

It is intended to be **understandable**.

If this repository helps you go from *"I have no idea how a chess engine works"* to *"I understand how these pieces fit together — and I want to build my own"*, then it has done its job.

Read the book.

Read the code.

Run the engine.

Play through the example games.

Break something.

Fix it.

Then make ABC stronger — or write your own engine.

**Happy chess programming.**
