#include "gol_transform_base.h"

void GolTransformBase::removeChild(GolTransformBase* child) {
    GolTransformBase* parent = this;
    GolTransformBase* head = parent->child;
    if (head == child) {
        parent->child = child->next;
        child->parent = NULL;
        child->next = NULL;
        return;
    }
    parent = head;
    GolTransformBase* nxt = parent->next;
    while (nxt != NULL) {
        if (nxt == child) {
            parent->next = nxt->next;
            nxt->parent = NULL;
            nxt->next = NULL;
            return;
        }
        parent = nxt;
        nxt = nxt->next;
    }
}

void GolTransformBase::addChild(GolTransformBase* child) {
    if (child->parent == this)
        return;
    child->parent = this;
    child->next = this->child;
    this->child = child;
}

GolTransformBase::GolTransformBase() {
    this->parent = NULL;
    this->next = NULL;
    this->child = NULL;
}
