#include "rbtree.h"

#include <stdlib.h>

void left_rotate(rbtree *tree, node_t *node);

void right_rotate(rbtree *tree, node_t *node);

void rb_insert_fixup(rbtree *tree, node_t *node);

rbtree *new_rbtree(void) {
    rbtree *tree = (rbtree *) calloc(1, sizeof(rbtree));
    // TODO: initialize struct if needed
    node_t *nil = (node_t *) calloc(1, sizeof(node_t));
    nil->color = RBTREE_BLACK;
    tree->root = tree->nil = nil;
    return tree;
}

void delete_rbtree(rbtree *t) {
    // TODO: reclaim the tree nodes's memory
    free(t);
}

void left_rotate(rbtree *tree, node_t *node) {
    node_t *right_child = node->right;
    node->right = right_child->left;

    if (right_child->left != tree->nil) {
        right_child->left->parent = node;
    }

    right_child->parent = node->parent;

    if (node->parent == tree->nil) {
        tree->root = right_child;
    } else if (node == node->parent->left) {
        node->parent->left = right_child;
    } else {
        node->parent->right = right_child;
    }

    right_child->left = node;
    node->parent = right_child;
}

void right_rotate(rbtree *tree, node_t *node) {
    node_t *left_child = node->left;
    node->left = left_child->right;

    if (left_child->right != tree->nil) {
        left_child->right->parent = node;
    }

    left_child->parent = node->parent;

    if (node->parent == tree->nil) {
        tree->root = left_child;
    } else if (node == node->parent->right) {
        node->parent->right = left_child;
    } else {
        node->parent->left = left_child;
    }
    left_child->right = node;
    node->parent = left_child;
}

void rb_insert_fixup(rbtree *tree, node_t *node) {
    while (tree->root != node && node->parent->color == RBTREE_RED) {
        if (node->parent == node->parent->parent->left) {
            node_t *uncle = node->parent->parent->right;
            if (uncle->color == RBTREE_RED) {
                uncle->color = RBTREE_BLACK;
                node->parent->color = RBTREE_BLACK;

                node->parent->parent->color = RBTREE_RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    left_rotate(tree, node);
                }
                node->parent->color = RBTREE_BLACK;
                node->parent->parent->color = RBTREE_RED;
                right_rotate(tree, node->parent->parent);
            }
        } else {
            node_t *uncle = node->parent->parent->left;
            if (uncle->color == RBTREE_RED) {
                uncle->color = RBTREE_BLACK;
                node->parent->color = RBTREE_BLACK;

                node->parent->parent->color = RBTREE_RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    right_rotate(tree, node);
                }
                node->parent->color = RBTREE_BLACK;
                node->parent->parent->color = RBTREE_RED;
                left_rotate(tree, node->parent->parent);
            }
        }
    }
    tree->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert
    node_t *new_node = (node_t *) calloc(1, sizeof(node_t));

    new_node->key = key;
    new_node->color = RBTREE_RED;
    new_node->left = new_node->right = t->nil;

    node_t *parent = t->nil;
    node_t *current = t->root;
    while (current != t->nil) {
        parent = current;
        if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    new_node->parent = parent;

    if (parent == t->nil)
        t->root = new_node;
    else if (key < parent->key)
        parent->left = new_node;
    else
        parent->right = new_node;

    rb_insert_fixup(t, new_node);

    return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    // TODO: implement find
    return t->root;
}

node_t *rbtree_min(const rbtree *t) {
    // TODO: implement find
    return t->root;
}

node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
    // TODO: implement erase
    return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    return 0;
}
