# 1 "main.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "main.c"
int i = 0;
int off = 5;

void inc(void){
  i += off;
}


int main(void){
  while (1) {
    inc();
  }
}
