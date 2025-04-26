// Brainf interpreter, fully functional implementation, written in approximately 1 hour on 4/6/2024.

#include <stdio.h>

int main(int argc, char* argv[]) {
  // Terminate if we didn't get a filename from the command line.
  if (argc < 2) {
    printf("Error: insufficient arguments supplied. Usage: %s <filename>\n", argv[0]);
    return -1;
  }

  FILE* IP = fopen(argv[1], "r");
  
  // Terminate if we couldn't open the file.
  if (!IP) {
    printf("Error: couldn't open specified file.\n");
    return -1;
  }
  
  // Make our array of byte cells.
  unsigned char data[30000];
  
  // Initialize the data cells to 0.
  for (int i = 0; i < sizeof(data); i++) {
    data[i] = 0;
  }
  
  // Data pointer, stores the current data cell. Starts at cell 0.
  unsigned long long int DP = 0;
  
  // The buffer for the current instruction.
  unsigned char buffer;
  
  // Main interpreter loop.
  while (1) {
    buffer = getc(IP);
    
    // Stop if we hit the end of the file.
    if (feof(IP)) break;
    
    // Keep going if we got a non-instruction character.
    if ((buffer != '>') && (buffer != '<') && (buffer != '+') && (buffer != '-') && (buffer != '.') && (buffer != ',') && (buffer != '[') && (buffer != ']')) continue;
    
    switch (buffer) {
      case '+':
        data[DP]++;
        break;
      case '-':
        data[DP]--;
        break;
      case '>':
        DP++;
        break;
      case '<':
        // Prevent invalid memory from being accessed.
        if (DP < 0) {
          printf("Error: tried to access memory below first byte.\n");
          fclose(IP);
          return -1;
        }
        DP--;
        break;
      case '.':
        printf("%c", data[DP]);
        break;
      case ',':
        data[DP] = getchar();
        break;
      case '[':
        if (data[DP] == 0) {
          // Read until the next matching ] character is reached.
          unsigned int ignore = 0;
          while (1) {
            buffer = getc(IP);
            
            if (feof(IP)) break;
            
            if (buffer == '[') {
              ignore++;
            }
            else if (buffer == ']') {
              if (ignore) {
                ignore--;
              }
              else {
                break;
              }
            }
          }
        }
        break;
      case ']':
        if (data[DP] != 0) {
          unsigned int ignore = 0;
          while (1) {
            fseek(IP, -2, SEEK_CUR);
            
            buffer = getc(IP);
            
            if (buffer == ']') {
              ignore++;
            }
            else if (buffer == '[') {
              if (ignore) {
                ignore--;
              }
              else {
                break;
              }
            }
          }
        }
    }
  }
  
  fclose(IP);

  return 0;
}
