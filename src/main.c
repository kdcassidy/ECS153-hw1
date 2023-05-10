#include "APInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LEN 256

int main(int argc, char const *argv[]) {

  // Open the input and output files, use stdin and stdout if not configured.
  FILE *output = NULL;
  int outputGiven = (argc >= 3);
  if (!outputGiven)
    output = stdout;
  else
    output = fopen(argv[2], "w");

  FILE *input = NULL;
  int inputGiven = (argc >= 2);
  if (!inputGiven)
    input = stdin;
  else
    input = fopen(argv[1], "r");


  /* Your code to init APInt array, and operate on them here. */
  
  // This takes in one line at a time
  char *iLine = NULL; // previously [MAX_LEN] (gave errors)
  size_t buflen = 0;
  int n = 0;

  bool error = false;
  
  // Take n of ints, declares n sized array of APInts
  if(getline(&iLine, &buflen, input) == -1)
    error = true;
  n = atoi(iLine);
  APInt *apint_array[n]; // does not need to be malloced or freed

  // Receive 2n number of commands
  // First line is error checked. Each number / index is guaranteed to be valid
  for (int i = 0; i < n; i++) {
    // input type
    if(getline(&iLine, &buflen, input) == -1) { // Convoluded setup to avoid warn_unused_result
      error = true;
      break;
    }
    clean_input_str(iLine);
    if (!strcmp(iLine, "UINT64")) {
      if(getline(&iLine, &buflen, input) == -1) {
        error = true;
        break;
      }
      clean_input_str(iLine);
      APInt *new_apint = uint64_to_APInt(atoi(iLine));
      if (new_apint == NULL) {
        printf("NULL returned\n");
      } else {
        apint_array[i] = new_apint;
      } 
    } else if (!strcmp(iLine, "HEX_STRING")) {
      if(getline(&iLine, &buflen, input) == -1){
        error = true;
        break;
      }
      clean_input_str(iLine);
      APInt *new_apint = hex_to_APInt(iLine);
      if (new_apint == NULL) {
        printf("NULL returned\n");
      } else {
        apint_array[i] = new_apint;
      } 
    } else if (!strcmp(iLine, "CLONE")) {
      if(getline(&iLine, &buflen, input) == -1) {
        error = true;
        break;
      }
      clean_input_str(iLine);
      APInt *new_apint = clone_APInt(apint_array[atoi(iLine)]);
      if (new_apint == NULL) {
        printf("NULL returned\n");
      } else {
        apint_array[i] = new_apint;
      } 
    } else {
      error = true;
      break;
    }
  }

  while (!error) {
    // get command
    if(getline(&iLine, &buflen, input) == -1) {
      error = true; // same purpose but consistent
      break;
    }
    clean_input_str(iLine);
    if (!strcmp(iLine, "END")) {
      break;
    } else if (!strcmp(iLine, "DUMP")) {
      for (int i = 0; i < n; i++) {
        (void)print_APInt(apint_array[i], output);
      }
    } else if (!strcmp(iLine, "SHL")) {
      if(getline(&iLine, &buflen, input) == -1) {
        error = true;
        break;
      }
      clean_input_str(iLine);
      int dst = atoi(strtok(iLine, " "));
      int src = atoi(strtok(NULL, " "));
      uint64_t k = (uint64_t)strtoull(strtok(NULL, " "), NULL, 10);
      left_shift_APInt(apint_array, dst, src, k);
    } else if (!strcmp(iLine, "ADD")) {
      if(getline(&iLine, &buflen, input) == -1) {
        error = true;
        break;
      }
      clean_input_str(iLine);
      int dst = atoi(strtok(iLine, " "));
      int op1 = atoi(strtok(NULL, " "));
      int op2 = atoi(strtok(NULL, " "));
      add_APInts(apint_array, dst, op1, op2);
    } else if (!strcmp(iLine, "MUL_UINT64")) {
      if(getline(&iLine, &buflen, input) == -1) {
        error = true;
        break;
      }
      clean_input_str(iLine);
      int dst = atoi(strtok(iLine, " "));
      int src = atoi(strtok(NULL, " "));
      uint64_t k = (uint64_t)strtoull(strtok(NULL, " "), NULL, 10);
      mul_UInt(apint_array[dst], apint_array[src], k);
    } else if (!strcmp(iLine, "MUL_APINT")) {
      if(getline(&iLine, &buflen, input) == -1) {
        error = true;
        break;
      }
      clean_input_str(iLine);
      int dst = atoi(strtok(iLine, " "));
      int op1 = atoi(strtok(NULL, " "));
      int op2 = atoi(strtok(NULL, " "));
      mul_APInts(apint_array[dst], apint_array[op1], apint_array[op2]);
    } else if (!strcmp(iLine, "POW")) {
      if(getline(&iLine, &buflen, input) == -1) {
        error = true;
        break;
      }
      clean_input_str(iLine);
      int dst = atoi(strtok(iLine, " "));
      int src = atoi(strtok(NULL, " "));
      uint64_t k = (uint64_t)strtoull(strtok(NULL, " "), NULL, 10);
      pow_APInt(apint_array[dst], apint_array[src], k);
    } else if (!strcmp(iLine, "CMP")) {
      if(getline(&iLine, &buflen, input) == -1) {
        error = true;
        break;
      }
      clean_input_str(iLine);
      int op1 = atoi(strtok(iLine, " "));
      int op2 = atoi(strtok(NULL, " "));
      cmp_APInts(apint_array[op1], apint_array[op2], output);
    } else {
      error = true;
      break;
    }
  }

  for (int i = 0; i < n; i++) {
    destroy_APInt(apint_array[i]);
  }
  free(iLine);
  
  // Close the files we opened.
  if (outputGiven)
    fclose(output);
  if (inputGiven)
    fclose(input);
  return 0;
}
