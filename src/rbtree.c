#include "rbtree.h"

#include <stdlib.h>

void left_rotate(rbtree *tree, node_t *node);

void right_rotate(rbtree *tree, node_t *node);

void rb_insert_fixup(rbtree *tree, node_t *node);

node_t *tree_minimum(const rbtree *t, node_t *node);

void rb_transplant(rbtree *t, node_t *u, node_t *v);

void rb_erase_fixup(rbtree *tree, node_t *node);

int in_order_tree_walk(const rbtree *t, node_t *root, key_t *arr, int i);

void post_order_delete_rbtree(rbtree *t, node_t *root);

rbtree *new_rbtree(void) {
    rbtree *tree = (rbtree *) calloc(1, sizeof(rbtree));
    // TODO: initialize struct if needed
    node_t *nil = (node_t *) calloc(1, sizeof(node_t));
    nil->color = RBTREE_BLACK;
    tree->root = tree->nil = nil;
    return tree;
}

void post_order_delete_rbtree(rbtree *t, node_t *root) {
    if (root != t->nil) {
        post_order_delete_rbtree(t, root->left);
        post_order_delete_rbtree(t, root->right);
        free(root);
    }
}

void delete_rbtree(rbtree *t) {
    // TODO: reclaim the tree nodes's memory
    post_order_delete_rbtree(t, t->root);
    free(t->nil);
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
    node_t *current = t->root;
    while (current != t->nil) {
        if (key == current->key) {
            return current;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL;
}

node_t *rbtree_min(const rbtree *t) {
    // TODO: implement find
    node_t *current = t->root;
    while (current->left != t->nil) {
        current = current->left;
    }
    return current;
}

node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    node_t *current = t->root;
    while (current->right != t->nil) {
        current = current->right;
    }
    return current;
}

node_t *tree_minimum(const rbtree *t, node_t *node) {
    node_t *current = node;
    while (current->left != t->nil) {
        current = current->left;
    }
    return current;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v) {
    if (u->parent == t->nil) {
        t->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

void rb_erase_fixup(rbtree *tree, node_t *replacement) {
    while (replacement != tree->root && replacement->color == RBTREE_BLACK) {
        if (replacement == replacement->parent->left) {
            node_t *sibling = replacement->parent->right;
            if (sibling->color == RBTREE_RED) {
                sibling->color = RBTREE_BLACK;
                replacement->parent->color = RBTREE_RED;
                left_rotate(tree, replacement->parent);
                sibling = replacement->parent->right;
            }
            if (sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) {
                sibling->color = RBTREE_RED;
                replacement = replacement->parent;
            } else {
                if (sibling->right->color == RBTREE_BLACK) {
                    sibling->left->color = RBTREE_BLACK;
                    sibling->color = RBTREE_RED;
                    right_rotate(tree, sibling);
                    sibling = replacement->parent->right;
                }
                sibling->color = replacement->parent->color;
                replacement->parent->color = RBTREE_BLACK;
                sibling->right->color = RBTREE_BLACK;
                left_rotate(tree, replacement->parent);
                replacement = tree->root;
            }
        } else {
            node_t *sibling = replacement->parent->left;
            if (sibling->color == RBTREE_RED) {
                sibling->color = RBTREE_BLACK;
                replacement->parent->color = RBTREE_RED;
                right_rotate(tree, replacement->parent);
                sibling = replacement->parent->left;
            }
            if (sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK) {
                sibling->color = RBTREE_RED;
                replacement = replacement->parent;
            } else {
                if (sibling->left->color == RBTREE_BLACK) {
                    sibling->right->color = RBTREE_BLACK;
                    sibling->color = RBTREE_RED;
                    left_rotate(tree, sibling);
                    sibling = replacement->parent->left;
                }
                sibling->color = replacement->parent->color;
                replacement->parent->color = RBTREE_BLACK;
                sibling->left->color = RBTREE_BLACK;
                right_rotate(tree, replacement->parent);
                replacement = tree->root;
            }
        }
    }
    replacement->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *delete_node) {
    // TODO: implement erase

    node_t *successor = delete_node;
    color_t successor_original_color = successor->color;
    node_t *replacement;

    if (delete_node->left == t->nil) {
        replacement = delete_node->right;
        rb_transplant(t, delete_node, delete_node->right);
    } else if (delete_node->right == t->nil) {
        replacement = delete_node->left;
        rb_transplant(t, delete_node, delete_node->left);
    } else {
        successor = tree_minimum(t, delete_node->right);
        successor_original_color = successor->color;
        replacement = successor->right;
        if (successor->parent == delete_node) {
            replacement->parent = successor;
        } else {
            rb_transplant(t, successor, successor->right);
            successor->right = delete_node->right;
            successor->right->parent = successor;
        }
        rb_transplant(t, delete_node, successor);
        successor->left = delete_node->left;
        successor->left->parent = successor;
        successor->color = delete_node->color;
    }

    free(delete_node);

    if (successor_original_color == RBTREE_BLACK) {
        rb_erase_fixup(t, replacement);
    }
    return 0;
}

int in_order_tree_walk(const rbtree *t, node_t *root, key_t *arr, int i) {
    if (root != t->nil) {
        i = in_order_tree_walk(t, root->left, arr, i);
        arr[i] = root->key;
        i += 1;
        i = in_order_tree_walk(t, root->right, arr, i);
    }
    return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    if (t->root != t->nil) {
        in_order_tree_walk(t, t->root, arr, 0);
    }
    return 0;
}
