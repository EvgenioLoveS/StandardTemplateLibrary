template <typename T>
class MyStack {
private:
    struct Node {
        T data;
        Node* next;
    };
    Node* root;

public:
    MyStack() : root(nullptr) { };
    ~MyStack()
    {
        while(!empty()){
            pop();
        }
    }

    void push(T x){
        Node* NewNode = new Node;
        NewNode->data=x;
        NewNode->next=root;
        root=NewNode;
    }

    T pop(){
        if(empty()){
            return 0;
        }
        Node* DelNode = root;
        T x = DelNode->data;
        root = DelNode->next;
        delete DelNode;
        return x;
    }

    bool empty() const{
        return root==nullptr;
    }

    const T& top(){
        if(empty()){
            return 0;
        }
        return root->data;
    }


};
