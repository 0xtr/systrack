FILE *fp_err;
FILE *fp_mem;

int setup (void) {
    /* set up / create the error log file */
    if (access("logs/errlog.txt", F_OK) == -1) {
        fp_err = fopen("logs/errlog.txt", "w+");
        if (fp_err == NULL) {
            perror("Couldn't create the error log.\n");         // not a good start
            exit(EXIT_FAILURE);
        }
        fclose(fp_err);
    }
    /* redirect stderr to the text file for this */
    fp_err = freopen("logs/errlog.txt", "a", stderr);
    fseek(fp_err, 0, SEEK_END);
    if (fp_err == NULL) {
        perror("Couldn't open stderr.\n");
        exit(EXIT_FAILURE);
    }
    /* set up / create the memory log file */
    if (access("logs/memlog.txt", F_OK) == -1) {
        fp_mem = fopen("logs/memlog.txt", "a+");
        if (fp_mem == NULL) {
            log_issue("Couldn't create memlog."); 
            exit(EXIT_FAILURE);
        }
    } else {
        fp_mem = fopen("logs/memlog.txt", "a+");
    }
    return EXIT_SUCCESS;
}
