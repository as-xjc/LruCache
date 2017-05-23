#pragma once

#include <cstdint>
#include <map>
#include <functional>

template <typename k, typename v>
class Lru {
 private:
  struct Node {
    Node* Pre{nullptr};
    Node* Next{nullptr};
    k Key;
    v Value;
    Node(k key, v value):Key(key), Value(value) {}
  };

 public:
  Lru(std::size_t cap = 8) : capacity_(cap) {}
  virtual ~Lru() { Clear(); }

  std::size_t GetCapacity() { return capacity_; }

  std::size_t GetSize() { return map_.size(); }

  void Clear() { while (DeleteTail()) {} }

  virtual void ForEach(std::function<void (v& value)> foreach) final {
    if (root_ == nullptr) return;

    Node* p = root_;
    do {
      foreach(p->Value);
      p = p->Next;
    } while (p != root_);
  }

  virtual void Push(k key, v value) final {
    if (map_.size() >= capacity_) DeleteTail();

    auto p = new Node(key, value);
    PushFront(p);

    map_.insert(std::make_pair(key, p));
  }

  v Find(k key) {
    auto it = map_.find(key);
    if (it == map_.end()) return v();

    Node* p = it->second;
    if (p != root_) {
      RemoveNode(p);
      PushFront(p);
    }
    return p->Value;
  }

 protected:
  virtual void onDelete(k key, v value) {}

 private:
  virtual bool DeleteTail() final {
    if (root_ == nullptr) return false;

    auto tail = root_->Pre;
    RemoveNode(tail);
    map_.erase(tail->Key);
    onDelete(tail->Key, tail->Value);
    delete tail;
    return true;
  }

  virtual void RemoveNode(Node* p) final {
    if (p == nullptr) return;
    if (map_.size() == 1) {
      root_ = nullptr;
    } else {
      auto tail = root_->Pre;
      if (root_ == p) {
        auto second = root_->Next;
        second->Pre = tail;
        tail->Next = second;
        root_ = second;
      } else if (tail == p) {
        auto tailpre = p->Pre;
        root_->Pre = tailpre;
        tailpre->Next = root_;
      } else {
        auto pre = p->Pre;
        auto next = p->Next;
        pre->Next = next;
        next->Pre = pre;
      }
    }
    p->Next = nullptr;
    p->Pre = nullptr;
  }

  virtual void PushFront(Node* p) final {
    if (root_ == nullptr) {
      root_ = p;
      root_->Pre = root_;
      root_->Next = root_;
    } else {
      auto tail = root_->Pre;
      root_->Pre = p;
      tail->Next = p;
      p->Next = root_;
      p->Pre = tail;
      root_ = p;
    }
  }

  virtual void PushBack(Node *p) final {
    if (root_ == nullptr) {
      p->Pre = p;
      p->Next = p;
      root_ = p;
    } else {
      auto tail = root_->Pre;

      tail->Next = p;
      p->Pre = tail;
      p->Next = root_;
      root_->Pre = p;
    }
  }

  Node* root_{nullptr};
  std::map<k, Node*> map_;
  const std::size_t capacity_;
};
