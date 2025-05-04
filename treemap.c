#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL) return;
    if(searchTreeMap(tree, key) != NULL) return;
    TreeNode * new = createTreeNode(key, value);
    if (new == NULL) return;
    if (tree->root == NULL) {
        tree->root = new;
        tree->current = new;
        return;
    }
    TreeNode * current = tree->root;
    TreeNode * parent = NULL;
    while (current != NULL) {
        parent = current;
        if (tree->lower_than(new->pair->key, current->pair->key)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    new->parent = parent;
    if (tree->lower_than(new->pair->key, parent->pair->key)) {
        parent->left = new;
    } else {
        parent->right = new;
    }
    tree->current = new;
}

TreeNode * minimum(TreeNode * x){
    while(x->left != NULL)
    {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode* temp = node;
    TreeNode* parent = temp->parent;
    if(temp == NULL)
        return;

    if(temp->left == NULL && temp->right == NULL) {
        if(temp != tree->root) {
            if(parent->left == temp)
                parent->left = NULL;
            else
                parent->right = NULL;
        } else {
            tree->root = NULL;
        }

        free(temp);
    }
    // Caso 2: el nodo tiene un solo hijo
    else if(temp->left == NULL || temp->right == NULL) {
        TreeNode* child;
        if(temp->left != NULL)
            child = temp->left;
        else
            child = temp->right;

        if(temp != tree->root) {
            if(parent->left == temp){
                parent->left = child;
                child->parent = parent;
            }
            else{
                parent->right = child;
                child->parent = parent;}
        } else {
            tree->root = child;
        }

        free(temp);
    }
    else {
        TreeNode* successor = minimum(temp->right);
        void* succKey = successor->pair->key;
        void* succData = successor->pair->value;
        removeNode(tree, successor);
        temp->pair->key = succKey;
        temp->pair->value = succData;
    }
}


void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    while(aux != NULL){
        if(is_equal(tree, aux->pair->key, key)){
            tree->current = aux;
            return aux->pair;
        }
        else if(tree->lower_than(key, aux->pair->key)){
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    TreeNode * result = NULL;

    while(aux != NULL){
        if(tree->lower_than(key, aux->pair->key)) {
            result = aux;
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }

    return result ? result->pair : NULL;
}


Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = minimum(tree->root);
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;
    TreeNode * aux = tree->current;
    if (aux->right != NULL) {
        aux = minimum(aux->right);
        tree->current = aux;
        return aux->pair;
    } else {
        while (aux->parent != NULL && aux == aux->parent->right) {
            aux = aux->parent;
        }
        tree->current = aux->parent;
        return tree->current ? tree->current->pair : NULL;
    }

}
