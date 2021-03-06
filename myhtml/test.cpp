#include <iostream>
#include <myhtml/api.h>
#include <fstream>
#include <sstream>
using namespace std;
mystatus_t serialization_callback(const char* data, size_t len, void* ctx) {

    printf("%.*s\n", (int)len, data);
    return MyCORE_STATUS_OK;
}
int main() {
    //读取文件
    ifstream in("/root/bet365.html", ios::in);
    stringstream buffer;
    buffer << in.rdbuf();
    string file(buffer.str());
    char* html;
    const int len = file.length();
    html = new char[len + 1];
    strcpy(html, file.c_str());
    // basic init
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    // first tree init
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);

    // parse html
    myhtml_parse_fragment(tree, MyENCODING_UTF_8, html, strlen(html),
                          MyHTML_TAG_DIV, MyHTML_NAMESPACE_HTML);
    // myhtml_parse(tree, MyENCODING_UTF_8, html, strlen(html));

    // print result
    // mycore_string_raw_t str = {0};
    // myhtml_serialization_tree_buffer(myhtml_tree_get_document(tree), &str);
    // cout << str.data << endl;
    //查找某个标签的
    // myhtml_collection_t *liList = myhtml_get_nodes_by_name(tree, NULL ,"p",
    // 3, NULL);
    // cout << liList << endl;
    // return 0;
    // if (liList && liList -> list && liList -> length){
    //     cout << liList -> length << endl;
    //     myhtml_tree_node_t *liNode = myhtml_node_child(liList -> list[0]);
    //     if (liNode){
    //         const char * text = myhtml_node_text(liNode, NULL);
    //         if (text)
    //             cout << text << endl;
    //     }
    // }
    //查找key
    const char* str = "description";
    myhtml_collection_t* collection = myhtml_get_nodes_by_attribute_key(
        tree, NULL, NULL, str, strlen(str), NULL);
    ofstream out("/root/betvictor", ios::app);
    for (size_t i = 0; i < collection->length; i++) {
        myhtml_tree_node_t* node = collection->list[i];
        if (node) {
            myhtml_tree_attr_t* getChar = myhtml_node_attribute_first(node);
            while (getChar) {
                const char* attrChar = myhtml_attribute_value(getChar, NULL);
                out << attrChar << ",";
                // cout << attrChar << endl;
                getChar = myhtml_attribute_next(getChar);
            }
            out << endl;
            const char* child = "bvs-button-sport market-outcome";
            myhtml_collection_t* childCollection =
                myhtml_get_nodes_by_attribute_value_whitespace_separated(
                    tree, NULL, node, true, "class", 5, child, strlen(child),
                    NULL);
            for (size_t i = 0; i < childCollection->length; i++) {
                myhtml_tree_node_t* childNode =
                    myhtml_node_child(childCollection->list[i]);
                while (childNode) {
                    myhtml_tree_node_t* textNode = myhtml_node_child(childNode);
                    if (textNode) {
                        const char* text = myhtml_node_text(textNode, NULL);
                        if (text) {
                            out << text << ":";
                            //cout << text << endl;
                        }
                    }
                    childNode = myhtml_node_next(childNode);
                }
                out << "************************";
            }
            out << endl;
            // cout << childCollection->length << endl;
            // string attr = "description";
            // string attr = "date";
            // myhtml_tree_attr_t *gets_attr = myhtml_attribute_by_key(node,
            // attr.c_str(),attr.length());
            // const char*attr_char = myhtml_attribute_value(gets_attr, NULL);
            // cout << attr_char << endl;
        }
    }

    // release resources
    myhtml_collection_destroy(collection);
    // mycore_string_raw_destroy(&str, false);
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);

    return 0;
}
