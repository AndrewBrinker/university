#include <stdlib.h>
#include <string.h>

typedef struct sym_tbl_record sym_tbl_record;

struct sym_tbl_record {
    char *name;
    int type;
    union {
        double var;
        function_t function_ptr;
    } value;
    struct sym_tbl_record *next;
};

sym_tbl_record *symbol_table = 0;


/**
 * Add a new symbol to the symbol table, and return a pointer to it.
 *
 * In this implementation, the symbol table is a linked list of individual
 * records. This function acts like a push_front() on that linked list, which
 * can be implemented most efficiently without a supervisory data structure.
 *
 * @param  sym_name -> The name of the new symbol.
 * @param  sym_type -> The type of the new symbol.
 * @return a pointer to the newly created symbol.
 */
sym_tbl_record *putsym(const char *sym_name, int sym_type) {
    sym_tbl_record *ptr = (sym_tbl_record *) malloc(sizeof(sym_tbl_record));
    ptr->name = (char *) malloc(strlen(sym_name) + 1);
    strcpy(ptr->name, sym_name);
    ptr->type = sym_type;
    ptr->value.var = 0;
    ptr->next = (sym_tbl_record *) symbol_table;
    symbol_table = ptr;
    return ptr;
}


/**
 * Searches through the symbol table for the symbol with the given name.
 *
 * @param  sym_name -> The name of the symbol being searched for.
 * @return a pointer to the given symbol if present, 0 otherwise.
 */
sym_tbl_record *getsym(const char *sym_name) {
    sym_tbl_record *ptr;
    for (ptr = symbol_table;
        ptr != (sym_tbl_record *) 0;
        ptr = (sym_tbl_record *) ptr->next) {
        if (strcmp(ptr->name, sym_name) == 0) {
            return ptr;
        }
    }
    return 0;
}
