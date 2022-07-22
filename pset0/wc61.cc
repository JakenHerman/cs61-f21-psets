#include <cstdio>
#include <iostream>

int main() {
    unsigned long n_bytes = 0;
    unsigned long n_words = 0;
    unsigned long n_lines = 0;

    char c;
    bool reading_word = false;
    while ((c = fgetc(stdin)) != EOF) {
        ++n_bytes;
        if (isspace(c)) {
          // if we were reading a word, this is a new space.
          // add to n_words and reset reading_word.
          if (reading_word) {
            ++n_words;
            reading_word = false;
          }
        } else {
          reading_word = true;
        }

        if (c == '\n') {
            ++n_lines;
        }
    }

    // if the loop ended while we were reading a word, count it.
    if (reading_word) {
      ++n_words;
    }

    // if we have words, but no lines, that's impossible. Add a line.
    if (n_words != 0 && n_lines == 0) {
      n_lines++;
    }

    fprintf(stdout, "%8lu\t%8lu\t%8lu\n", n_lines, n_words, n_bytes);
}
