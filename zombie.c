
#define DURATION 60
int main() {

    pid_t pid_fils;
    pid_fils = fork();
    if (pid_fils == 0) {
        for(i=DURATION; i>0; i - - ) 
        {
            printf( "Fils : je suis vivant pour encore %d secondes.\n", i);
            fflush(stdout); sleep(1);
        } 
    }
    return EXIT_SUCESS;
}