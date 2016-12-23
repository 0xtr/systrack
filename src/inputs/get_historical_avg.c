int get_historical_avg (int args, char **cmd) {
    int array_len = 0;
    int i;
    int retval = 0;

    for (i = 0; i < args; ++i) {
        array_len += strlen(cmd[i]);
    }
    array_len += 20;

    char *str = malloc(array_len);
    assert(str != NULL);
    memset(str, '\0', array_len);

    strcpy(str, "logs/_");
    for (i = 1; i < args; ++i) {
        if (strstr(cmd[i], "./") != NULL) {
            strncat(str, &cmd[i][2], strlen(cmd[i]) - 2);
        } else {
            strncat(str, cmd[i], strlen(cmd[i]));
        }
    }
    strcat(str, ".txt");

    if (access(str, F_OK) != -1) {
        cmdinfo = NULL;
        cmdinfo = fopen(str, "r");
        assert(cmdinfo != NULL);

        char *line = NULL;
        ssize_t rv = 0;
        size_t n = 0;
        rv = getline(&line, &n, cmdinfo);
        assert(rv > 0);

        char *split = strtok(line, ",");
        data.runs = atoi(split);
        assert(data.runs > 0);
        split = strtok(NULL, ",");
        assert(split != NULL);
        data.total = atoi(split);
        assert(data.total > 0);

        retval = fclose(cmdinfo);
        assert(retval == 0);
        cmdinfo = NULL;
        cmdinfo = fopen(str, "w");
        assert(cmdinfo != NULL);
    } else {
        cmdinfo = NULL;
        cmdinfo = fopen(str, "w+");
        assert(cmdinfo != NULL);
        data.runs = 0;
        data.total = 0;
    }
    free(str);
    return EXIT_SUCCESS;
}
