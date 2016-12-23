void log_issue (char str[]) {
    struct tm tmp;
    time_t t = time(NULL); 
    if (str[strlen(str)] == '\n') {
        str[strlen(str)] = '\0'; 
    }
    localtime_r(&t, &tmp);
    fprintf(stdout, "%lu | %d/%d - %d:%d | %s | %s\n", 
            t,
            tmp.tm_mday, tmp.tm_mon + 1, tmp.tm_hour, tmp.tm_min,
            str, strerror(errno));
    fprintf(stderr, "%lu | %d/%d - %d:%d | %s | %s\n", 
            t,
            tmp.tm_mday, tmp.tm_mon + 1, tmp.tm_hour, tmp.tm_min,
            str, strerror(errno));
}
