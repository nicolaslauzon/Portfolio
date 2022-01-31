#ifndef SLITERATOR_H
#define SLITERATOR_H
#include "SlNode.h"

template <typename T>
class SlIterator {
private:
    SlNode<T>* node_;
    SlNode<T>* first_;
public:
    SlIterator(SlNode<T>* node){
        node_ = first_ = node;
    }
    void Iterate(){
        if(node_)
            node_ = node_->next;
    }
    bool End(){ //if at the end return "false", if not return "true"
        if(node_ == first_)
            return false;
        return true;
    }
    const T& Data() const {
        return node_->data;
    }
};
#endif // SLITERATOR_H
