//=================================================================
// Implementation for DLL module.
//
// Copyright 2021 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//=================================================================
#include <stdlib.h>
#include <stdio.h>
#include "doubly_linked_list.h"

LLNode* create_llnode(void* data) {
    LLNode* newNode = (LLNode*)malloc(sizeof(LLNode));
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->data = data;
    return newNode;
}

DLinkedList* create_dlinkedlist(void) {
    DLinkedList* newList = (DLinkedList*)malloc(sizeof(DLinkedList));
    newList->head = NULL;
    newList->tail = NULL;
    newList->size = 0;
    return newList;
}

int getSize(DLinkedList* dLinkedList) {
    return dLinkedList->size;
}

LLNode* getHead(DLinkedList* dLinkedList) {
    return dLinkedList->head;
}

LLNode* getTail(DLinkedList* dLinkedList) {
    return dLinkedList->tail;
}

LLNode* getNext(LLNode* node) {
    return node->next;
}

LLNode* getPrev(LLNode* node) {
    return node->prev;
}

void* getData(LLNode* node) {
    return node->data;
}

void insertAfter(DLinkedList* dLinkedList, LLNode* prev_node, void* data) {
    if (prev_node == NULL) {
    printf("the given previous node cannot be NULL");
    return;
    }
    LLNode* newNode = create_llnode(data);
    newNode->next = prev_node->next;
    newNode->prev = prev_node;
    prev_node->next = newNode;
    if (prev_node == dLinkedList->tail) {
        dLinkedList->tail = newNode;
        dLinkedList->size++;
        return;
    }
    newNode->next->prev = newNode;
    dLinkedList->size++;
    return;
}

void insertBefore(DLinkedList* dLinkedList, LLNode* next_node, void* data) {
    if (next_node == NULL) {
        printf("the given next node cannot be NULL");
        return;
    }
    LLNode* newNode = create_llnode(data);
    newNode->prev = next_node->prev;
    newNode->next = next_node;
    next_node->prev = newNode;
    if (next_node == dLinkedList->head) {
        dLinkedList->head = newNode;
        dLinkedList->size++;
        return;
    }
    next_node->prev->next = newNode;
    dLinkedList->size++;
    return;
}

void insertHead(DLinkedList* dLinkedList, void* data) {
    if(dLinkedList->head == NULL) {
        LLNode* newNode = create_llnode(data);
        dLinkedList->head = newNode;
        dLinkedList->tail = newNode;
        dLinkedList->size++;
    } else {
        insertBefore(dLinkedList, dLinkedList->head, data);
    }
}

void insertTail(DLinkedList* dLinkedList, void* data) {
    if(dLinkedList->tail == NULL) {
        LLNode* newNode = create_llnode(data);
        dLinkedList->head = newNode;
        dLinkedList->tail = newNode;
        dLinkedList->size++;
    } else {
        insertAfter(dLinkedList, dLinkedList->tail, data);
    }
}

void deleteNode(DLinkedList* dLinkedList, LLNode* Node) {
    LLNode* temp = NULL;
    if (dLinkedList == NULL) {
        return;
    }
    if (dLinkedList->head == dLinkedList->tail) {
        dLinkedList->head = NULL;
        dLinkedList->tail = NULL;
    } else if (Node == dLinkedList->head) {
        Node->next->prev = NULL;
        dLinkedList->head = Node->next;
    } else if (Node == dLinkedList->tail) {
        Node->prev->next = NULL;
        dLinkedList->tail = Node->prev;
    } else {
        temp = Node->next;
        Node->prev->next = Node->next;
        temp->prev = Node->prev;
    }
    dLinkedList->size--;
    free(Node->data);
    free(Node);
    return;
}

void destroyList(DLinkedList* dLinkedList) {
    LLNode* del = dLinkedList->head;
    while (del != NULL) {
        LLNode* current = del->next;
        deleteNode(dLinkedList, del);
        del = current;
    }
    free(dLinkedList);
    return;
}

void reverse(DLinkedList* dLinkedList) {
    if (getSize(dLinkedList) == 0) {
        return;
    }
    LLNode* current = dLinkedList->head;
    LLNode* temp = NULL;
    while (current != NULL) {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev; 
    }
    while (temp != NULL) {
        dLinkedList->tail = dLinkedList->head;
        dLinkedList->head = temp->prev;
        temp = NULL;
    }
    return;
}