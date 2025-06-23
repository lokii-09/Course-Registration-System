#include <bits/stdc++.h>
using namespace std;
class student{
  public:
   string name;
   int year;
   string id;
   unordered_map<string,int> comp;
   unordered_map<string,int> enrolled;
   unordered_map<char,int> slots;
};
class course{
    public:
    string code;
    string name;
    int credits;
    int capacity;
    int currcap=0;
    vector<string> pre;
    char slot;
    vector<string> en;
    queue<string> waitList;
};
int main(){
    unordered_map<string,student> pupil;
    unordered_map<string,course> courses;
    int q;
    cin>>q;
    while(q--){
        string s;
        cin>>s;
        if(s=="add_student"){
            student newStudent;

            cin >> newStudent.id;
            cin >> newStudent.name;
            cin >> newStudent.year;

            int numCompleted;
            cin>>numCompleted;
            
            for(int i=0;i<numCompleted;i++){
                string courseCode;
                cin>>courseCode;
                newStudent.comp[courseCode]++;
            }
            pupil[newStudent.id]=newStudent;
        }
        else if(s=="add_course"){
            course newCourse;

            cin>>newCourse.code;
            cin>>newCourse.name;
            cin>>newCourse.credits;
            cin>>newCourse.capacity;
            cin>>newCourse.slot;

            int numPrerequisites;
            cin>>numPrerequisites;

            bool canAdd=true;

            for(int i=0;i<numPrerequisites;i++){
                string prerequisite;
                cin>>prerequisite;

                newCourse.pre.push_back(prerequisite);

                if(courses.find(prerequisite)==courses.end()){
                    canAdd=false;
                }
            }
            if(canAdd){
                courses[newCourse.code]=newCourse;
            }
        }
        else if(s=="enroll"){
            string studentId;
            cin>>studentId;

            auto studentSearch= pupil.find(studentId);

            if(studentSearch==pupil.end()) {string courseCode;cin>>courseCode;}

            else {
                string courseCode;
                cin>>courseCode;
                auto courseSearch=courses.find(courseCode);

                if(courseSearch==courses.end()){}
                else {
                    if(courseSearch->second.currcap<courseSearch->second.capacity){
                        int numPrerequisites=courseSearch->second.pre.size();
                        int hasAllPrereqs=0;

                        auto& studentRef=studentSearch->second;
                        auto& courseRef=courseSearch->second;

                        for(int i=0;i<numPrerequisites;i++){
                            if(studentRef.comp.find(courseRef.pre[i])==studentRef.comp.end()) {hasAllPrereqs=1;break;}
                        }

                        if(hasAllPrereqs==0) {
                            if(studentRef.enrolled.find(courseRef.code)==studentRef.enrolled.end()){
                                if(studentRef.slots.find(courseRef.slot)==studentRef.slots.end()) {
                                    studentRef.slots[courseRef.slot]++;
                                    courseRef.currcap++;
                                    courseRef.en.push_back(studentRef.id);
                                    studentRef.enrolled[courseRef.code]++;
                                }
                            }
                        }
                    }
                    else {
                        int numPrerequisites=courseSearch->second.pre.size();
                        int hasAllPrereqs=0;

                        auto& studentRef=studentSearch->second;
                        auto& courseRef=courseSearch->second;

                        for(int i=0;i<numPrerequisites;i++){
                            if(studentRef.comp.find(courseRef.pre[i])==studentRef.comp.end()) {hasAllPrereqs=1;break;}
                        }
                        if(hasAllPrereqs==0) {
                            if(studentRef.enrolled.find(courseRef.code)==studentRef.enrolled.end()){
                                if(studentRef.slots.find(courseRef.slot)==studentRef.slots.end()) {
                                    courseRef.waitList.push(studentRef.id);
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(s=="drop"){
            string studentId;
            cin>>studentId;

            string courseCode;
            cin>>courseCode;

            auto studentSearch= pupil.find(studentId);
            auto courseSearch=courses.find(courseCode);

            if(studentSearch==pupil.end() || courseSearch==courses.end()){}
            else {
                auto& studentRef=studentSearch->second;
                auto& courseRef=courseSearch->second;

                if(studentRef.enrolled.find(courseCode)!=studentRef.enrolled.end()){
                    studentRef.enrolled.erase(courseCode);
                    studentRef.slots.erase(courseRef.slot);
                    courseRef.currcap--;

                    for(auto it=courseRef.en.begin();it!=courseRef.en.end();++it){
                        if(*it==studentId){
                            courseRef.en.erase(it);
                            break;
                        }
                    }

                    while(!courseRef.waitList.empty() && courseRef.currcap<courseRef.capacity){
                        string waitingStudentId=courseRef.waitList.front();
                        courseRef.waitList.pop();
                        auto waitingStudentSearch=pupil.find(waitingStudentId);

                        if(waitingStudentSearch==pupil.end()){
                            continue;
                        }

                        auto& waitingStudentRef=waitingStudentSearch->second;
                        bool isEligible=true;

                        if(waitingStudentRef.enrolled.find(courseCode)!=waitingStudentRef.enrolled.end()){
                            isEligible=false;
                        }

                        if(isEligible){
                            for(int i=0;i<courseRef.pre.size();i++){
                                if(waitingStudentRef.comp.find(courseRef.pre[i])==waitingStudentRef.comp.end()){
                                    isEligible=false;
                                    break;
                                }
                            }
                        }

                        if(isEligible && waitingStudentRef.slots.find(courseRef.slot)!=waitingStudentRef.slots.end()){
                            isEligible=false;
                        }

                        if(isEligible){
                            waitingStudentRef.slots[courseRef.slot]++;
                            courseRef.currcap++;
                            courseRef.en.push_back(waitingStudentRef.id);
                            waitingStudentRef.enrolled[courseCode]++;
                            break;
                        }
                    }
                }
            }
        }
        else if(s=="print"){
            string courseCode;
            cin>>courseCode;

            if(courses.find(courseCode)==courses.end()) cout<<"Invalid Course"<<" "<<courseCode<<endl;
            else {
                auto courseRef=courses.find(courseCode)->second;
                for(int i=0;i<courseRef.en.size();i++){
                    if(i==0) cout<<"Enrolled students in "<<courseCode<<":"<<endl;
                    cout<<courseRef.en[i]<<endl;
                }
            }
        }
    }
}