
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

class node{
public:
    int val;
    node* parent;
    node* left;
    node* right;
    node(int v, node* l, node* r, node* p){
        this->val = v;
        this->left=l;
        this->right=r;
    
    };
    
    void insert(int v){ 
        node* p = this;
        node* n_tmp=new node(v,NULL,NULL,NULL);
        int flag=1;
        while(flag==1){
            if(p->val > v){
                if(p->left==NULL){
                    p->left=n_tmp;
                    n_tmp->parent=p;
                    flag=0;
                }else{
                    p->left->parent=p;
                    p=p->left;
                }
            }else{
                if(p->right==NULL){
                    p->right=n_tmp;
                    n_tmp->parent=p;
                    flag=0;
                }else{
                    p->right->parent=p;
                    p=p->right;
                }
            }
            
        }
    }; //method insert end

    void del(char lr){
        node* n=this;
        if(n->left==NULL&&n->right==NULL){
            if(lr=='r'){
                n->parent->right=NULL;
            }
            else if(lr=='l'){
                n->parent->left=NULL;
            }
        }
        else if(n->left != NULL && n->right == NULL){
            if(lr=='r'){
                n->left->parent=n->parent;
                n->parent->right=n->left;
            }
            else if(lr=='l'){
                n->left->parent=n->parent;
                n->parent->left=n->left;
            }
        }
        else if(n->right != NULL && n->left == NULL){
            if(lr=='r'){
                n->right->parent=n->parent;
                n->parent->right=n->right;
            }
            else if(lr=='l'){
                n->right->parent=n->parent;
                n->parent->left=n->right;
            }
        }
        else if(n->right != NULL && n->left != NULL){
            node* del_cand;
            
            node* p=n->right;
            node* p_parent=n;
            
            if(p->left==NULL){ //n -> r -> r
                n->val=p->val;
                p_parent->right=NULL;
                if(p->right!=NULL){
                    p->right->parent=p_parent;
                    p_parent->right=p->right;
                }
                return;
            }   
            
            // n ->  l- r -r -> r
            int flag=1;
            while(flag==1){
                if(p->left==NULL){
                    flag=0;
                }
                else if(p->left!=NULL){
                    p_parent=p;
                    p=p->left;
                }
            }
            n->val=p->val;
            p_parent->right=NULL;
            if(p->right!=NULL){
                p->right->parent=p_parent;
                p_parent->left=p->right;
            }
        }
    }; //method delete end
    void print(){ 
        node* p = this;
        cout<<p->val<<endl;
        if(p->left!=NULL){
            node* p_l=p->left;
            p_l->print();
        }
        if(p->right!=NULL){
            node* p_r=p->right;  
            p_r->print();
        }
    }; //method print end
    
};


node** makeTree(vector<int> v, int start, int K){
    node* root = new node(K,NULL,NULL,NULL);
    node** head = &root;
    
    for(int i=start;i<v.size();i++){
        if(v[i]>=K)
            break;
        root->insert(v[i]);
    }
    
    return head;
}

void mixScv(node** tree, int K, int& m_cnt){
    node* min= (*tree)->left;
    
    if(min==NULL && m_cnt>0){
        
            m_cnt=-1;
            return;
    }else if(min==NULL && m_cnt==0){
            return;
    }
    node* min2= (*tree)->left->right;
    int flag=1;
    int cnt=0;
    while(flag==1){
        if(min2==NULL && min->val<K){
            m_cnt=-1;
            return;
        }
        m_cnt++;
        int mixed=min->val+(min2->val*2);
        min2->del('r');
        
        min2=NULL;
        if(min->right==NULL){
            (*tree)->left=NULL;
        }
        else{
            min->val=min->right->val;
            min2 = min->right;

            min2->del('r');

            if(min->right==NULL){
                min2=NULL;
            }
        }
       
        if(mixed<K){
            (*tree)->insert(mixed);
            if(min2==NULL && min!=NULL){
                min2=min->right;
            }else if(min2==NULL && min==NULL)
                min=(*tree)->left;
        }
         
        if((*tree)->left==NULL){
            return;
        }
    } 
}


int solution(vector<int> scv, int K) {
    int answer = 0;
    sort(scv.begin(),scv.end());
    vector<int> scv_org;
    scv_org.resize(scv.size());
    copy(scv.begin(),scv.end(), scv_org.begin());

    ///// 0 ?????? ???????????? /////
    int m_cnt=0;
    int i = 0;
    while(scv_org[i] == 0){
        m_cnt++;
        i++;
    }
    
     ///// tree ????????? /////
    node** head = makeTree(scv,i,K);
    
    //// scoville ?????? ?????? /////
    mixScv(head,K,m_cnt);
    
    answer=m_cnt;
    return answer;
}


/*
    ***note ***********************************
    
    GOAL : ?????? ????????? ????????? ????????? K ????????????.
    
    METHOD : mixed_scovile = min_scovile + ( second_min_scovile * 2 )
    
    RETURN : ?????? ?????? scovile 'K'?????? ?????? ?????? ?????? #mix??????
    
    CONDITION : (1) ?????? ?????? : (scovile.size()) -> 2??????, 1,000,000??????.
                (2) ??? ?????? ?????? scovile ?????? -> 0??????, 1,000,000 ??????.
                (3) ?????? K : 0??????, 1,000,000,000 ??????
                (4) ?????? ?????? K ?????? ?????? case : -1 return
    
    
    *******************************************

*/
