#include <vector>


// Just container class
class Node {
public:
    Node() : m_parent(nullptr) {}
    virtual ~Node() {}

    void addChild(Node* child)
    {
        child->m_parent = this;
        m_children.push_back(child);
    }

    void removeChild(Node* child) 
    {
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it != m_children.end())
        {
            m_children.erase(it);
            child->m_parent = nullptr;
        }
    }

    void update(float dt) 
    {
        // update this node
        onUpdate(dt);

        // update child nodes
        for (auto child : m_children) 
        {
            child->update(dt);
        }
    }

    Node* getParent() const { return m_parent; }

protected:
    virtual void onUpdate(float dt) {}

private:
    Node* m_parent;
    std::vector<Node*> m_children;
};
