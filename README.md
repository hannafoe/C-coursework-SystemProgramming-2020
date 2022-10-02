# Overview
This is a coursework in C. There were 4 parts:
- 1 Makefiles and dynamic-linked libraries
In this part we learned to make makefiles and link libraries dynamically.
- 2 Implement Connect 4 Twist & Turn Game
Connect 4 Twist & Turn is a game played by two players ('x' and 'o') on a rectangular grid that is wrapped round a cylinder (so the \leftmost" column is one to the right of the \rightmost" column). Rows are numbered 1; : : : ; n bottom-to-top and columns are numbered 1; : : : ;m left to right. Each cell of the grid can either contain a token of a player (either 'x' or 'o') or it can be empty (denoted '.'). The cylinder stands upright, so if a token is in a column, but the position underneath is empty, then the token will automatically drop down into that space by gravity.
- 3 Reflective Report
Write a 1-2 page report describing and justifying the approaches you used for the above code and on how the code in the main.c and connect4.h could be improved.
- 4 Command-line sort program
In this part of the coursework, you will implement a simplified version of the UNIX sort command. Your
code must use the C function qsort() to do the actual sorting and you must use a switch statement to parse the command-line options (in particular, you may not use getopt or qsort_r(), as they are not part
of the C11 standard).

Please refer to SpecificationOfTask.pdf for more information on the task. Refer to the code and the report for my actual implementation.