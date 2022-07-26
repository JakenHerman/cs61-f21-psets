#include <cstring>
#include <cassert>
#include <cstdio>

char* mystrstr(const char* s1, const char* s2, const char* s3) {
    char c = *s3;
    if (c == '1') {
      if (!s2[0]) {
        return (char*)s1;
      }
        for (int i = 0; s1[i] != '\0'; i++) {
          int j = 0;
          while (s2[j] != '\0' && s2[j + i] == s1[1]) {
           j++;
          }
         return (char*)&s1[i];
        }
        return nullptr;
        // bracket notation
    } else if (c == '2') {
        
    }
    
    return nullptr; 
}

int main(int argc, char* argv[]) {
    assert(argc == 4);
    printf("strstr(\"%s\", \"%s\") = %p\n",
           argv[1], argv[2], strstr(argv[1], argv[2]));
    printf("mystrstr(\"%s\", \"%s\") = %p\n",
           argv[1], argv[2], mystrstr(argv[1], argv[2], argv[3]));
    assert(strstr(argv[1], argv[2]) == mystrstr(argv[1], argv[2], argv[3]));
}