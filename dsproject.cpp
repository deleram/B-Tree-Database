#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <algorithm>
using namespace std;
#define int long long

class BTNode;
class BTree;

class Node{
    public:
        int data;
        BTNode * self;
        Node * nextField = NULL;
};

class BTNode{
    public:
        BTNode(int data, Node * node){
            for(int i = 0; i < 3; i++){
                value[i] = NULL;
            }
            for(int i = 0; i < 4; i++){
                child[i] = NULL;
            }
            SetVal(data, node);
            return;
        }
        void SetVal(int val, Node * node){
            for(int i = 0; i < 3; i++){
                if(value[i] == val){
                    self[i].push_back(node);
                    break;
                }
                if(value[i] == NULL){
                    value[i] = val;
                    self[i].push_back(node);
                    break;
                }
            }
            SortValue();
            return;
        }
    private:
        int * value = new int [3];
        BTNode ** child = new BTNode* [4];
        BTNode * par = NULL;
        vector <Node *> * self = new vector <Node *> [3];
        void SortValue(){
            for(int i = 1; i < 3; i++){
                int tmp = i;
                while(tmp != 0 && value[tmp] != NULL && (value[tmp - 1] == NULL || value[tmp - 1] > value[tmp])){
                    swap(value[tmp], value[tmp - 1]);
                    swap(self[tmp], self[tmp - 1]);
                    tmp --;
                }
            }
            return;
        }
    friend class BTree;
};

class BTree{
    public:
        BTree(){
            head = NULL;
        }
        BTNode* Find_node(int val){
            return Finding(val, head, NULL);
        }


        void Insert_node(int val, Node * node){
            BTNode * cur = Find_node(val);
            if(cur != NULL){
                for(int i = 0; i < 3; i++){
                    if(cur -> value[i] == val){
                        cur -> SetVal(val, node);
                        return;
                    }
                }
            }
            vector <Node *> vnode;
            vnode.push_back(node);
            Inserting(cur, val, vnode);
            return;
        }

        void Delete_node(int val, Node * node){
            BTNode * cur = Find_node(val);
            if(cur != NULL){
                for(int i = 0; i < 3; ++i){
                    if(cur -> value[i] == val){
                        if((cur -> self[i]).size() > 1){
                            vector <Node *> tmp;
                            while(!(cur -> self[i]).empty() && (cur -> self[i]).back() != node){
                                tmp.push_back((cur -> self[i]).back());
                                (cur -> self[i]).pop_back();
                            }
                            if(!(cur -> self[i]).empty()) (cur -> self[i]).pop_back();

                            while(!tmp.empty()){
                                (cur -> self[i]).push_back(tmp.back());
                                tmp.pop_back();
                            }
                            return;
                        }
                        for(int j = 0; j < 4 ; ++j){
                            if(cur -> child[j] != NULL){
                                Delete_non_leaf(cur , val);
                                return;
                            }
                        }
                        Delete_leaf(cur , val);
                        return;
                    }
                }
            }
            //cout << "NOT FOUND" << endl;
            return;
        }

        void Print_Tree(){
            Printing(head);
        }

        vector <Node *> Find1(int val){
            ans_find.clear();
            Find1_helper(val, head);
            return ans_find;
        }

        vector <Node *> Find2(int val){
            ans_find.clear();
            Find2_helper(val, head);
            return ans_find;
        }

        vector <Node *> Find3(int val){
            ans_find.clear();
            Find3_helper(val, head);
            return ans_find;
        }


    private:

        void Printing(BTNode * cur){
            if(cur == NULL){
                return;
            }
            for(int i = 0; i < 3; i++){
                if(cur -> value[i] == NULL){
                    break;
                }
                for(int j = 0; j < (int)(cur -> self[i]).size(); j++) cout << cur -> self[i][j] -> data << " ";
            }
            cout << endl;
            for(int i = 0; i < 4; i++){
                Printing(cur -> child[i]);
            }
            return;
        }


        void Inserting(BTNode * cur, int val, vector <Node *> node){
            if(cur == NULL){
                head = new BTNode(val, node[0]);
                for(int i = 1; i < (int)(node.size()); i++){
                    (head -> self[0]).push_back(node[i]);
                }
                return;
            }
            cur -> SetVal(val, node[0]);
            for(int i = 0; i < 3; i++){
                if(cur -> value[i] == val){
                    for(int j = 1; j < (int)node.size(); j++){
                        (cur -> self[i]).push_back(node[j]);
                    }
                    break;
                }
            }
            if(cur -> value[2] == NULL){
                return;
            }
            BTNode *L = new BTNode(cur -> value[0], cur -> self[0][0]);
            for(int i = 1; i < (int)(cur -> self[0]).size(); i++){
                (L -> self[0]).push_back(cur -> self[0][i]);
            }
            BTNode *R = new BTNode(cur -> value[2], cur -> self[2][0]);
            for(int i = 1; i < (int)(cur -> self[2]).size(); i++){
                (R -> self[0]).push_back(cur -> self[2][i]);
            }
            L -> child[0] = cur -> child[0];
            if(cur -> child[0] != NULL){
                (cur -> child[0]) -> par = L;
            }
            L -> child[1] = cur -> child[1];
            if(cur -> child[1] != NULL){
                (cur -> child[1]) -> par = L;
            }
            L -> par = cur -> par;

            R -> child[0] = cur -> child[2];
            if(cur -> child[2] != NULL){
                (cur -> child[2]) -> par = R;
            }
            R -> child[1] = cur -> child[3];
            if(cur -> child[3] != NULL){
                (cur -> child[3]) -> par = R;
            }
            R -> par = cur -> par;

            BTNode * tmppar = cur -> par;
            if(tmppar == NULL){
                head = new BTNode(cur -> value[1], cur -> self[1][0]);
                for(int i = 1; i < (int)(cur -> self[1]).size(); i++){
                    (head -> self[0]).push_back(cur -> self[1][i]);
                }
                head -> child[0] = L;
                head -> child[1] = R;
                L -> par = head;
                R -> par = head;
                return;
            }
            int L_index = 0;
            for(int i = 0; i < 4; i++){
                if(tmppar -> child[i] == cur){
                    tmppar -> child[i] = L;
                    L_index = i;
                }
            }
            if(tmppar -> child[L_index + 1] == NULL){
                tmppar -> child[L_index + 1] = R;
            }else{
                int temp = 3;
                while(temp != L_index + 1){
                   swap(tmppar -> child[temp],tmppar -> child[temp - 1]);
                   -- temp;
                }
                tmppar -> child[L_index + 1] = R;
            }
            Inserting(tmppar, cur-> value[1], cur -> self[1]);
            return;
        }

        BTNode* Finding(int val, BTNode* current, BTNode* par){
            if(current == NULL){
                return par;
            }
            int last = 0;
            for(int i = 0; i < 3; i++){
                if(current -> value[i] == val){
                    return current;
                }
                if(current -> value[i] > val){
                    return Finding(val, current -> child[i],current);
                }
                if(current -> value[i] == NULL){
                    last = i;
                    break;
                }
            }
            return Finding(val, current -> child[last], current);
        }

        void Delete_leaf(BTNode *current , int val){
            int index = 0;
            for(int i = 0; i < 3 ; ++i){
                if(current -> value[i] == val){
                    index = i;
                }
            }

            if(current -> value[1] != NULL){
                current -> value[index] = NULL;
                current -> self[index].clear();
                current->SortValue();
                return;
            }

            if(current == head){
                current -> value[index] = NULL;
                current -> self[index].clear();
                current ->SortValue();
                if(index == 0){
                    for(int i = 0; i < 3; ++i){
                        if (current -> child[i] != NULL){
                            head = current -> child[i];
                            (current -> child[i]) -> par = NULL;
                        }
                    }
                }
                return;
            }

            int index2 = 0;
            BTNode *parent = current -> par;
            for(int i = 0; i < 4; ++i){
                if(parent -> child[i] == current){
                    index2 = i ;
                }
            }
            if(index2 != 0){
                if (((parent -> child [index2 -1]) != NULL) && (parent -> child [index2 -1]) -> value[1] != NULL){
                    current -> value[0] = NULL;
                    current -> self[0].clear();
                    swap((parent -> child[index2 - 1]) -> value[1] , parent -> value[index2 -1]);
                    swap((parent -> child[index2 - 1]) -> self[1] , parent -> self[index2 -1]);
                    current -> value[0] = (parent -> child[index2 - 1]) -> value[1];
                    current -> self[0] = (parent -> child[index2 - 1]) -> self[1];
                    (parent -> child[index2 - 1]) -> value[1] = NULL;
                    (parent -> child[index2 - 1]) -> self[1].clear();
                    if (current -> child[0] == NULL){
                        current -> child[0] = (parent -> child[index2 - 1]) -> child [2];
                    }else{
                        current -> child [1] = current -> child[0];
                        current -> child[0] = (parent -> child[index2 - 1]) -> child [2];
                    }
                    (parent -> child[index2 - 1]) -> child [2] = NULL;
                    return;
                }
            }

            if(index2 != 2){
                if (((parent -> child [index2 +1]) != NULL) && (parent -> child [index2 +1]) -> value[1] != NULL){
                    current -> value[0] = NULL;
                    current -> self[0].clear();
                    swap((parent -> child[index2 +1]) -> value[0] , parent -> value[index2]);
                    swap((parent -> child[index2 +1]) -> self[0] , parent -> self[index2]);
                    current -> value[0] = (parent -> child[index2 +1]) -> value[0];
                    current -> self[0] = (parent -> child[index2 +1]) -> self[0];
                    (parent -> child[index2 + 1]) -> value[0] = NULL;
                    (parent -> child[index2 + 1]) -> self[0].clear();
                    (parent -> child[index2 + 1]) -> SortValue();
                    if(current -> child[1] == NULL){
                        current -> child[1] = (parent -> child[index2 + 1]) -> child[0];
                    }else{
                        current -> child [0] = current -> child[1];
                        current -> child[1] = (parent -> child[index2 - 1]) -> child [0];
                    }
                    (parent -> child[index2 + 1]) -> child[0] = (parent -> child[index2 + 1]) -> child[1];
                    (parent -> child[index2 + 1]) -> child[1] = (parent -> child[index2 + 1]) -> child[2];
                    return;
                }
            }

            if(index2 != 0){
                int tmp = parent -> value [index2 -1];
                vector <Node *> tmpnode = parent -> self [index2 -1];
                for(int i = 0; i < 3 ; ++i){
                    if (current -> child[i] != NULL){
                        current -> child [2] = current -> child[i];
                        break;
                    }
                }
                current -> child[0] = parent -> child[index2 -1] -> child[0];
                current -> child[1] = parent -> child[index2 -1] -> child[1];
                current -> value[0] = parent -> child[index2 -1] -> value[0];
                current -> value [1] = parent -> value[index2 -1];
                current -> self[0] = parent -> child[index2 -1] -> self[0];
                current -> self[1] = parent -> self[index2 -1];
                parent -> child[index2 - 1] = current;
                if (index2 == 1){
                    parent -> child[1] = parent -> child[2];
                }
                parent -> child[2] = NULL ;
                return Delete_leaf(parent , tmp);
            }

            if(index2 == 0){
                int tmp = 0;
                tmp = parent -> value [0];
                current -> value[1] = parent -> child[1] -> value[0];
                current -> value [0] = parent -> value [0];
                current -> self[1] = parent -> child[1] -> self[0];
                current -> self[0] = parent -> self [0];
                current -> child[1] = (parent -> child[1])-> child[0];
                current -> child[2] = (parent -> child[1])-> child[1];
                parent -> child[0] = current;
                parent -> child[1] = parent -> child[2];
                parent -> child[2] = NULL ;
                return Delete_leaf(parent , tmp);
            }

        }

        void Delete_non_leaf(BTNode *current , int val){
            int index = 0 , index2 = 0;
            for(int i = 0; i < 3 ; ++i){
                if(current -> value[i] == val){
                    index = i;
                }
            }
            BTNode *pre = Find_previous(current, index);
            for(int j = 2; j >=0; --j){
                if(pre -> value[j] != NULL){
                    index2 = j;
                    break;
                }
            }
            swap(current -> value[index], pre -> value[index2]);
            swap(current -> self[index], pre -> self[index2]);
            return Delete_leaf(pre, val);
        }

        BTNode * Find_previous(BTNode * current, int index){

            BTNode *tmp = current -> child[index];
            return Find_rightest(tmp ,current);
        }

        BTNode * Find_rightest(BTNode* cur, BTNode* par){
            if (cur == NULL){
                return par;
            }
            for(int i = 1 ; i >= 0 ; --i){
                if((cur -> value[i]) != NULL){
                    return Find_rightest(cur -> child[i+1] , cur);
                }
            }
        }

        void Find2_helper(int val , BTNode* current){
            if(current == NULL){
                return;
            }
            int last = 0;
            for(int i = 0; i < 2; i++){
                if(current -> value[i] == val){
                    int j = 0;
                    while(j < (int)(current -> self[i]).size() && current -> self[i][j] != NULL){
                        ans_find.push_back(current -> self[i][j]);
                        ++j;
                    }
                    return;
                }
                if(current -> value[i] > val){
                    Find2_helper(val, current -> child[i]);
                    return;
                }
                if(current -> value[i] == NULL){
                    last = i;
                    break;
                }
            }
            Find2_helper(val, current -> child[last]);
            return;
        }

        void Find1_helper(int val , BTNode* current){
            if(current == NULL){
                return;
            }
            int ind = 0;
            while(current -> value[ind] != NULL && current -> value[ind] <= val){
                ind ++;
            }

            Find1_helper(val, current -> child[ind]);
            for(int i = ind; i < 2 && current -> value[i] != NULL; ++i){
                int j = 0;
                while(j < (int)(current -> self[i]).size() && current -> self[i][j] != NULL){
                    ans_find.push_back(current -> self[i][j]);
                    ++j;
                }
                Find1_helper(val , current->child[i+1]);
            }
            return;
        }

        void Find3_helper(int val , BTNode* current){
            if(current == NULL){
                return;
            }
            Find3_helper(val, current -> child[0]);
            for(int i = 0; i < 2; ++i){
                if(current -> value[i] != NULL && current -> value[i] < val){
                    int j = 0;
                    while(j < (int)(current -> self[i]).size() && current -> self[i][j] != NULL){
                        ans_find.push_back(current -> self[i][j]);
                        ++j;
                    }
                    Find3_helper(val , current->child[i+1]);
                }
            }
            return;
        }

        vector <Node *> ans_find;
        BTNode * head ;

};

void Reverse(string& str){
    int n = str.length();
    for(int i = 0; i < n / 2; i++){
        swap(str[i], str[n - i - 1]);
    }
    return;
}

int Hash_String(string input){
    int answer = 0;
    for(int i = 0; i < input.size() ; ++i){
        int temp = (int)input[i];
        if(temp < 58){
            temp -= 47;
        }else{
            temp -= 86;
        }
        answer = answer*37 + temp;
    }
    return answer;
}

string UnHash_String(int input){
    string answer = "";
    char ans[37] ={'0','0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    while(input != 0){
        int temp = input % 37;
        answer += ans[temp];
        input = input / 37;
    }
    Reverse(answer);

    return answer;

}

int Hash_Time(string input){
    int ans = 0;
    int index = 5;
    for(int i = 0; i< 4; ++i){
        if(input[i] != '/'){
            int temp = (int) input[i] -48;
            ans = ans * 10 + temp;
        }else{
            index = i + 1;
            break;
        }
    }
    if(input[index +1] == '/'){
        int temp = (int) input[index] - 48;
        ans = ans * 100 + temp;
        index = index + 2;
    }else{
        int temp = (int) input[index] - 48;
        ans = ans * 10 + temp;
        temp = (int) input[index + 1] - 48;
        ans = ans * 10 + temp;
        index += 3;
    }
    if(index + 1 == input.size()){
        int temp = (int) input[index] - 48;
        ans = ans *100 + temp;
    }else{
        int temp = (int) input[index] - 48;
        ans = ans * 10 + temp;
        temp = (int) input[index + 1] - 48;
        ans = ans * 10 + temp;
    }
    return (ans);

}

string UnHash_Time(int input){
    string answer_Y = "";
    string answer_M = "";
    string answer_D = "";
    char ans[10] = {'0','1','2','3','4','5','6','7','8','9'};
    int day = input % 100;
    input = input/100;
    int month = input % 100;
    input /= 100;
    int year = input;
    while(year != 0){
        int temp = year % 10;
        answer_Y += ans[temp];
        year = year / 10;
    }
    Reverse(answer_Y);
    while(month != 0){
        int temp = month % 10;
        answer_M += ans[temp];
        month = month / 10;
    }
    Reverse(answer_M);
    while(day != 0){
        int temp = day % 10;
        answer_D += ans[temp];
        day = day/ 10;
    }
    Reverse(answer_D);
    string answer = answer_Y + '/' + answer_M + '/' + answer_D;
    return answer;
}

class Table{
    public:
        int Wanted_Value = 0;
        void Create(string * inp, int cnt){
            Number_of_columns = ((cnt - 3) / 2) + 1;
            Columns = new BTree * [Number_of_columns];
            Name_of_columns = new string [Number_of_columns];
            Type_of_columns = new string [Number_of_columns];
            Name_of_columns[0] = "ID", Type_of_columns[0] = "int", Columns[0] = new BTree;
            for(int i = 3, j = 1; j < Number_of_columns; i += 2, j++){
                Name_of_columns[j] = inp[i];
                Type_of_columns[j] = inp[i + 1];
                Columns[j] = new BTree;
            }
            return;
        }

        void Insert(string * inp){
            Node ** NODES= new Node *[Number_of_columns];
            for(int i = 0; i < Number_of_columns; ++i){
                NODES[i] = new Node;
            }
            for(int i = 0; i < (Number_of_columns - 1); ++i){
                NODES[i] -> nextField = NODES[i + 1];
            }
            NODES[Number_of_columns - 1] -> nextField = NODES[0];

            if(First_in.empty()){
                NODES[0] -> data = ++ Number_of_rows;
            }else{
                NODES[0] -> data = First_in.top();
                First_in.pop();
            }

            Columns[0] -> Insert_node(NODES[0] -> data, NODES[0]);
            for(int i = 4, j = 1; j < Number_of_columns; i++, j++){
                int ans = 0;
                if(Type_of_columns[j] == "int"){
                    ans = stoi(inp[i]);
                }else if(Type_of_columns[j] == "string"){
                    ans = Hash_String(inp[i].substr(1,inp[i].size()-2));
                }else{
                    ans = Hash_Time(inp[i]);
                }
                NODES[j] -> data = ans;
                Columns[j] -> Insert_node(ans, NODES[j]);
            }
        }

        pair <string **, int>  Select(string * input, int index ,int size){
            bool *Check_list = Make_check_list(input, index);
            Wanted_Value = 0;
            for (int i = 0; i < Number_of_columns ; ++i){
                if(Check_list[i] == true) ++ Wanted_Value;
            }
            int *Condition = Compare(input[size - 1]);
            vector <Node *> ans_find;
            if(Condition[1] == -1){
                ans_find = (this -> Columns[Condition[0]]) -> Find3(Condition[2]);
            }else if(Condition[1] == 0){
                ans_find = (this -> Columns[Condition[0]]) -> Find2(Condition[2]);
            }else{
                ans_find = (this -> Columns[Condition[0]]) -> Find1(Condition[2]);
            }

            for(int i = 0; i < (Number_of_columns - Condition[0]); ++i){
                for(int j = 0; j < (int)ans_find.size(); ++j){
                    ans_find[j] = ans_find[j]->nextField;
                }
            }

            string** answer = new string*[ans_find.size()];
            for(int j = 0; j < ans_find.size(); ++j){
                answer[j] = new string[Wanted_Value + 1];
                answer[j][0] = to_string(ans_find[j] -> data);
                //ans_find[j] = ans_find[j] -> nextField;
            }
            int z = 1;
            for(int i = 0; i < Number_of_columns ; ++i){
                for(int j = 0; j < ans_find.size(); ++j){
                    if (Check_list[i]){
                        if(Type_of_columns[i] == "string"){
                            answer[j][z] = UnHash_String((ans_find[j] -> data));
                        }else if(Type_of_columns[i] == "int"){
                            answer[j][z] = to_string(ans_find[j] -> data);
                        }else{
                            answer[j][z] = UnHash_Time(ans_find[j] -> data);
                        }
                    }
                    ans_find[j] = ans_find[j] -> nextField;
                }
                if(Check_list[i]) ++z;
            }
            return make_pair(answer,ans_find.size());
        }

        void Delete(string* input){
            int *Condition = Compare(input[4]);
            vector <Node *> ans_find;
            if(Condition[1] == -1){
                ans_find = (this -> Columns[Condition[0]]) -> Find3(Condition[2]);
            }else if(Condition[1] == 0){
                ans_find = (this -> Columns[Condition[0]]) -> Find2(Condition[2]);
            }else{
                ans_find = (this -> Columns[Condition[0]]) -> Find1(Condition[2]);
            }

            for(int i = 0; i < (Number_of_columns - Condition[0]); ++i){
                for(int j = 0; j < (int)ans_find.size(); ++j){
                    ans_find[j] = ans_find[j]->nextField;
                }
            }

            for(int i = (int)ans_find.size() - 1; i >= 0; --i){
                for(int j = 0 ; j < Number_of_columns; ++j){                    
                    if(j == 0) First_in.push(ans_find[i] -> data);
                    (this ->Columns[j]) -> Delete_node(ans_find[i]-> data , ans_find[i]);
                    ans_find[i] = ans_find[i] -> nextField;

                }
            }
        }

        void Update(string* input , int size){
            int *Condition = Compare(input[size -1]);
            vector <Node *> ans_find;
            if(Condition[1] == -1){
                ans_find = (this -> Columns[Condition[0]]) -> Find3(Condition[2]);
            }else if(Condition[1] == 0){
                ans_find = (this -> Columns[Condition[0]]) -> Find2(Condition[2]);
            }else{
                ans_find = (this -> Columns[Condition[0]]) -> Find1(Condition[2]);
            }
            for(int i = 0; i < (Number_of_columns - Condition[0]); ++i){
                for(int j = 0; j < (int)ans_find.size(); ++j){
                    ans_find[j] = ans_find[j]->nextField;
                }
            }


            int node_size =(int)ans_find.size();
            int * IDs = new int[node_size];
            for(int j = 0; j < node_size; ++j){
                IDs[j] = ans_find[j] -> data;
            }
            for(int j = 0; j < node_size; ++j){
                for(int z = 0 ; z < Number_of_columns; ++z){                    
                    (this ->Columns[z]) -> Delete_node(ans_find[j]-> data , ans_find[j]);
                    ans_find[j] = ans_find[j] -> nextField;
                }
                Node ** NODES= new Node *[Number_of_columns];
                for(int i = 0; i < Number_of_columns; ++i){
                    NODES[i] = new Node;
                }
                for(int i = 0; i < (Number_of_columns - 1); ++i){
                    NODES[i] -> nextField = NODES[i + 1];
                }
                NODES[Number_of_columns - 1] -> nextField = NODES[0];
                NODES[0] -> data = IDs[j];            
                Columns[0] -> Insert_node(NODES[0] -> data, NODES[0]);

                for(int z = 1; z < Number_of_columns; z++){
                    int ans = 0;
                    if(Type_of_columns[z] == "int"){
                        ans = stoi(input[(2 + z)]);
                    }else if(Type_of_columns[z] == "string"){
                        ans = Hash_String(input[(2 + z)].substr(1,(input[(2 + z)].size())-2));
                    }else{
                        ans = Hash_Time(input[(2 + z)]);
                    }
                    NODES[z] -> data = ans;
                    Columns[z] -> Insert_node(ans, NODES[z]);
                }
            }

        }
    private:

    bool * Make_check_list(string * input, int index){
            bool * Check_list = new bool[Number_of_columns];
            for(int i = 0; i < Number_of_columns; ++i){
                Check_list[i] = false;
            }
            if (input[1] == "*"){
                for(int i = 0; i < Number_of_columns; ++i){
                    Check_list[i] = true;
                }
                return Check_list;
            }
            for(int i = 1; input[i] != "FROM"; ++i){
                for(int j = 0 ; j < Number_of_columns; ++j){
                    if(input[i] == Name_of_columns[j]){
                        Check_list[j] = true;
                        break;
                    }
                }
            }
            return Check_list;
    }

    int * Compare(string input){
        int* Condition = new int[3];
        for(int i = 0; i < 3; ++i) Condition[i] = 0;
        string ans1 = "", ans2 = "";
        int index = 0;
        for(int i = 0; i < input.size(); ++i){
            if(input[i] == '<'){
                Condition[1] = -1;
                ans1 = input.substr(0, i);
                ans2 = input.substr(i+1);
                break;
            }if(input[i] == '='){
                Condition[1] = 0;
                ans1 = input.substr(0, i);
                ans2 = input.substr(i+2);
                break;
            }if(input[i] == '>'){
                Condition[1] = 1;
                ans1 = input.substr(0, i);
                ans2 = input.substr(i+1);
                break;
            }
        }
        for(int i = 0; i < Number_of_columns; ++i){
            if(Name_of_columns[i] == ans1){
                Condition[0] = i;
                if(Type_of_columns[i] == "string"){
                    Condition[2] = Hash_String(ans2.substr(1,ans2.size()-2));
                }else if(Type_of_columns[i] == "int"){
                    Condition[2] = stoi(ans2);
                }else{
                    Condition[2] = Hash_Time(ans2);
                }
                return Condition;
            }
        }
    }

    int Number_of_columns = 0, Number_of_rows = 0;
    priority_queue <int, vector<int>, greater<int> > First_in;
    BTree ** Columns = NULL;
    string * Name_of_columns = NULL;
    string * Type_of_columns = NULL;



};

pair <string *, int> Split(string input){
    char c[4] = {' ', '(', ')', ','};
    bool * B = new bool[(int)input.size()];
    for(int i = 0; i < (int)input.size(); i++){
        B[i] = 0;
        for(int j = 0; j < 4; j++){
            if(input[i] == c[j]){
                B[i] = 1;
            }
        }
    }

    int Size = 0;
    for(int i = 0; i < (int)input.size(); i++){
        if(B[i]){
            if(i == 0 || B[i-1]) continue;
            Size ++;
        }
    }
    if(!B[(int)input.size() - 1]) Size ++;

    string * ans = new string[Size];
    string cur = "";
    int cnt = 0;
    for(int i = 0; i < (int)input.size(); i++){
        if(B[i]){
            if(cur == "") continue;
            ans[cnt] = cur;
            cur = "";
            cnt ++;
        }else{
            cur += input[i];
        }
    }
    if (cur != ""){
        ans[cnt] = cur;
    }
    return make_pair(ans, Size);
}

map <string, Table *> Table_Map;

int select_table(string * input, int Size){
    for(int i = 0; i < Size ; ++i){
        if(input[i] == "FROM"){
            return (i +1);
        }
    }
}

void Proccessor(string * inp, int size){
    if(inp[0] == "CREATE"){
        Table_Map[inp[2]] = new Table;
        Table_Map[inp[2]]->Create(inp , size);
        return;
    }
    if(inp[0] == "INSERT"){
        Table_Map[inp[2]] -> Insert(inp);
        return;
    }
    if(inp[0] == "SELECT"){
        int num = select_table(inp, size);
        pair <string **, int> answer ;
        answer = Table_Map[inp[num]] -> Select(inp, num,size);

        pair <int, string *> sort_answer[answer.second];
        for(int i = 0 ; i < answer.second ; ++i){
            sort_answer[i].first = stoi(answer.first[i][0]);
            sort_answer[i].second = answer.first[i];
        }
        sort(sort_answer, sort_answer + answer.second);

        for(int i = 0 ; i < answer.second ; ++i){
            for(int j = 1; j < (Table_Map[inp[num]] -> Wanted_Value + 1); ++j){
                cout << sort_answer[i].second[j] << " ";
            }
            cout << endl;
        }
        return;
    }
    if(inp[0] == "DELETE"){
        Table_Map[inp[2]] -> Delete(inp);
        return;
    }
    if(inp[0] == "UPDATE"){
        Table_Map[inp[1]] -> Update(inp, size);
    }
}


int32_t main(){
   /*
    BTree A;

    map <int, vector <Node *>> M;

    while(true){
        char c;
        int tmp;
        cin >> c >> tmp;
        if(c == 'A'){
            Node * tmpnode = new Node;
            M[tmp].push_back(tmpnode);
            (*tmpnode).data = tmp;
            A.Insert_node(tmp, tmpnode);
        }
        else{
            Node * tmpnode = M[tmp].back();
            M[tmp].pop_back();
            A.Delete_node(tmp, tmpnode);
        }
        A.Print_Tree();
    }

    return 0;
    */
    string n;
    getline(cin, n);
    int N = stoi(n);
    string* input = new string[N];
    for (int i = 0 ; i< N; ++i){
        getline(cin, input[i]);

    }
    for (int i = 0 ; i< N; ++i){
        pair <string *, int> splited;
        splited = Split(input[i]);
        Proccessor(splited.first ,splited.second);
    }
    return 0;
}
