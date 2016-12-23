int update_avg (void) {
    #define EMPTY_SZ 25
    size_t rv = 0;
    int status;
    char empty[EMPTY_SZ];
    char sndest[5];
    memset(empty, '\0', EMPTY_SZ);
    memset(sndest, '\0', 5);

    rewind(cmdinfo);
    rv = fwrite(&empty, 1, EMPTY_SZ, cmdinfo);
    if (rv <= 0) {
        log_issue("failed to write to cmdinfo for updating the avg data.");
        return -1;
    }
    rewind(cmdinfo);

    status = snprintf(empty, sizeof(int), "%d", data.runs);
    assert(status > 0);
    strncat(empty, ",", 1);
    status = snprintf(sndest, sizeof(int), "%d", data.total);
    strcat(empty, sndest);
    assert(status > 0);

    rv = fwrite(&empty, 1, EMPTY_SZ, cmdinfo);
    memset(empty, '\0', EMPTY_SZ);

    return EXIT_SUCCESS;
}

