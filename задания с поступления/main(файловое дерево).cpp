/*На вход поступают строки - пути к файлам.
Необходимо посчитать количество папок и файлов в каждой папке*/

#include <iostream>
#include <string> // подключаем строки
#include <fstream> // подключаем файлы
#include <vector>
#include <sstream>
using namespace std;

struct node{
    string name = "";
    vector<node*> dir_list;
};

int64_t pos_in_vector(vector<node*> in_vector, string in_name){
    for(u_int64_t i = 0; i < in_vector.size(); i++){
        if(in_vector[i]->name == in_name ){
            return i;
        }
    }
    return -1;
}

void print_dir(node *in_node_ptr, u_int8_t indent){
    for(u_int8_t i = 0; i < indent; i++){//рисуем отступ
        cout << " ";
    }
    if(in_node_ptr->dir_list.size() == 0){
        cout << "файл " << in_node_ptr->name << endl;
    } else {
        cout << "папка " << in_node_ptr->name << " c " << in_node_ptr->dir_list.size() << " элементами" << endl;
        for (u_int64_t i = 0; i < in_node_ptr->dir_list.size(); i++) {
            print_dir(in_node_ptr->dir_list[i], indent+4);
        }
    }
}

int main(){
    u_int64_t counter = 0; //введите, сколько строк.
    cin >> counter;
    string path;
    node *root_dir = new(node);
    root_dir->name = "root";

    for(u_int64_t i = 0; i < counter; i++){
        cin >> path;
        path += "/"; //чтоб не обрабатывать конец строки отдельно
        //разбор на директории
        u_int64_t last_end = 0;
        node *current_node = root_dir;
        for(u_int64_t j = 0; j < path.size(); j++){
            if(path[j] == '/'){
                string next_dir_name = path.substr(last_end, j - last_end);
                last_end = j+1;
                //обращаемся к текущей папке, если нет в vector,
                //то создаём node с названием
                //и добавляем в вектор
                //и переходим в новую node
                //иначе просто переходим
                int64_t pos = pos_in_vector(current_node->dir_list, next_dir_name);
                if(pos != -1){
                    current_node = current_node->dir_list[pos];
                } else{
                    node *new_node_ptr = new(node);
                    new_node_ptr->name = next_dir_name;
                    current_node->dir_list.push_back(new_node_ptr);
                    current_node = new_node_ptr;
                }
            };
        }
    }
    print_dir(root_dir, 0);

    return 0;
}
