int increment_record_for_ (int value) {
    call *crawler = head;
    int found = 0;
    while (crawler) {
        if (crawler->value == value) {
            found = 1;
            ++crawler->uses;
            break;
        }
        crawler = crawler->next;
    }
    if (found == 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
