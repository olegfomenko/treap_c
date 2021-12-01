#include <stdio.h>
#include <stdlib.h>

struct Node {
    int key, priority, sz;
    int value;
    struct Node *left, *right;
};

struct Pair {
    struct Node *left;
    struct Node *right;
};

int size(struct Node *node) {
    if (node == NULL) {
        return 0;
    }

    return node->sz;
}

void update_size(struct Node *node) {
    if (node != NULL) {
        node->sz = size(node->left) + size(node->right) + 1;
    }
}

struct Pair *split(struct Node *t, int k) {
    struct Pair *result = (struct Pair *) malloc(sizeof(struct Pair));
    result->left = NULL;
    result->right = NULL;

    if (t == NULL) {
        return result;
    }

    if (k <= t->key) {
        struct Pair *p = split(t->left, k);
        t->left = p->right;
        update_size(t);

        result->left = p->left;
        result->right = t;
    } else {
        struct Pair *p = split(t->right, k);
        t->right = p->left;
        update_size(t);

        result->left = t;
        result->right = p->right;
    }

    return result;
}


struct Node *merge(struct Node *t1, struct Node *t2) {
    if (t1 == NULL) return t2;
    if (t2 == NULL) return t1;

    if (t1->priority < t2->priority) {
        t2->left = merge(t1, t2->left);
        update_size(t2);
        return t2;
    } else {
        t1->right = merge(t1->right, t2);
        update_size(t1);
        return t1;
    }
}

extern struct Node *t;
struct Node *t;

int get(const int key) {
    struct Pair *p = split(t, key);
    struct Node *current = p->right;

    int result;

    while (1) {
        if (current == NULL) {
            exit(1);
        }

        if (current->key == key) {
            result = current->value;
            break;
        }

        current = current->left;
    }

    t = merge(p->left, p->right);
    return result;
}

void delete(const int key) {
    struct Pair *p = split(t, key);
    struct Node *current = p->right;

    if (current == NULL) {
        return;
    }

    if (current->key == key) {
        t = merge(p->left, merge(current->left, current->right));
        return;
    }

    while (1) {
        if (current->left == NULL) {
            break;
        }

        if (current->left->key == key) {
            free(current->left);
            current->left = NULL;
            update_size(current);
            break;
        }

        current = current->left;
    }

    t = merge(p->left, p->right);
}

void insert(const int key, const int val) {
    struct Node *node = (struct Node *) malloc(sizeof(struct Node));
    node->key = key;
    node->value = val;
    node->sz = 1;
    node->priority = rand();
    node->right = NULL;
    node->left = NULL;

    if (t == NULL) {
        t = node;
    } else {
        // TODO check duplications
        struct Pair *p = split(t, key);
        p->left = merge(p->left, node);
        t = merge(p->left, p->right);
    }
}

struct Node *unite(struct Node *t1, struct Node *t2) {
    if (t1 == NULL) return t2;
    if (t2 == NULL) return t1;

    if (t1->priority > t2->priority) {
        struct Pair *p = split(t2, t1->key);
        t1->left = unite(t1->left, p->left);
        t1->right = unite(t1->right, p->right);
        update_size(t1);
        return t1;
    } else {
        struct Pair *p = split(t1, t2->key);
        t2->left = unite(t2->left, p->left);
        t2->right = unite(t2->right, p->right);
        update_size(t2);
        return t2;
    }
}

int main() {
    insert(10, 1);
    insert(4, 5);
    insert(3, 121);

    printf("size=%d\n", size(t));

    printf("key=10 val=%d\n", get(10));
    printf("key=4 val=%d\n", get(4));
    printf("key=3 val=%d\n", get(3));

    delete(10);
    delete(4);

    printf("size=%d\n", size(t));
}
