This project converts a C source file into an HTML file that displays the contents of the C file on a webpage.
Each token in the code — such as operators, constants, keywords, and literals — is highlighted in a specific color, making the code more readable.

Project Workflow:
The project is implemented in four main steps:
  1. File Validation – Check whether the given file is a valid C file (based on extension).
  2. File Opening – Open and read the contents of the C file.
  3. Token Extraction – Identify and extract tokens such as keywords, operators, constants, and identifiers.
  4. HTML Generation – Write the tokens into an HTML file with appropriate colors for each token type.
