#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "TriviaDB.hpp"

using namespace std;
using namespace boost::property_tree;

boost::shared_ptr<TriviaDB> TriviaDB::db;

void TriviaDB::loadDB() {
    if (mQuestions.empty()) {
        ptree root;

        read_json(mLocation, root);

        // the json is an array of structs
        // It gets parsed with an empty string as key
        auto array = root.get_child("");

        for (auto it = array.begin(); it != array.end(); ++it) {
            Question question;

            // the data structure in the json array is -
            // {
            //   "question": "What is the meaning of life?",
            //   "A": "42",
            //   "B": "666",
            //   "C": "3.14",
            //   "D": "6.28",
            //   "answer": "A"
            // }
            question.question = "question: " + it->second.get<string>("question") + "\n";
            question.question += "A: " + it->second.get<string>("A") + "\n";
            question.question += "B: " + it->second.get<string>("B") + "\n";
            question.question += "C: " + it->second.get<string>("C") + "\n";
            question.question += "D: " + it->second.get<string>("D") + "\n";

            question.answer = it->second.get<string>("answer");

            mQuestions.push_back(question);
        }

        cout << "loaded " << mQuestions.size() << " questions from the database" << endl;
    }
}

