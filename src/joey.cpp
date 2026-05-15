#include "scene.h"

void Joey::removeChild(Joey* child) {
    Joey* parent = this;
    Joey* head = parent->child;
    if (head == child) {
        parent->child = child->next;
        child->parent = NULL;
        child->next = NULL;
        return;
    }
    parent = head;
    Joey* nxt = parent->next;
    if (nxt == NULL)
        return;
    do {
        if (nxt == child) {
            parent->next = nxt->next;
            nxt->parent = NULL;
            nxt->next = NULL;
            return;
        }
        parent = nxt;
        nxt = nxt->next;
    } while (nxt != NULL);
}

void Joey::addChild(Joey* child) {
    if (child->parent == this)
        return;
    child->parent = this;
    child->next = this->child;
    this->child = child;
}

Joey::Joey() {
    this->parent = NULL;
    this->next = NULL;
    this->child = NULL;
}
