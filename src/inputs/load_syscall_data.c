int load_syscall_data (void) {
    FILE *fp = NULL;
    int status = 0;
    if (access("src/syscalls.txt", F_OK) == -1) {
        return -1;
    }
    if ((fp = fopen("src/syscalls.txt", "r")) == NULL) {
        return -2;
    }
    ssize_t size = 0;
    size_t n = 0;
    char *line = NULL;
    char *token = NULL;
    rewind(fp);
    curr = NULL;
    head = NULL;
    while ((size = getline(&line, &n, fp)) != -1) {
        curr = (call *) malloc (sizeof(call));

        token = strtok(line, ",");
        assert(token != NULL);
        strncpy(curr->cname, token, MAX_CALL_NAME_LEN);

        token = strtok(NULL, ",");
        assert(token != NULL);
        curr->value = atoi(token);

        token = strtok(NULL, ",");
        assert(token == NULL);

        curr->next = head;
        head = curr;
    }
    curr = head;
    status = fclose(fp);
    assert(status == 0);
    return 0;
}
