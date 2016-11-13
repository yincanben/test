#include<iostream> 
#include<vector>

//get namespace related stuff 
using std::cin; 
using std::cout; 
using std::endl; 
using std::flush; 
using std::string; 
using std::vector; 
 
//struct Observer, modeled after java.utils.Observer 
struct Observer 
/* 
 * AK: This could be a template (C++) or generic (Java 5), 
 * however the original Smalltalk MVC didn't do that. 
 */ 
{ 
   //update 
   virtual void update(void*)=0; 
}; 
 
 //struct Observable, modeled after java.utils.Observable 
struct Observable 
{ 
   //observers 
   vector<Observer*>observers; 
   
   //addObserver 
   void addObserver(Observer*a){observers.push_back(a);} 
   
   //notifyObservers 
   void notifyObservers() 
   { 
    for (vector<Observer*>::const_iterator observer_iterator=observers.begin();observer_iterator!=observers.end();observer_iterator++) 
     (*observer_iterator)->update(this); 
   } 
  
  /* 
  AK: If you had a method which takes an extra "ARG" argument like this 
  notifyObservers(void* ARG), you can pass that arg to each Observer via 
  the call (*observer_iterator)->update(this,ARG); 
  
  
  This can significantly increase your View's reusablity down the track. 
  I'll explain why below in the View. 
  */

}; 
 
 
 //struct Model, contains string-data and methods to set and get the data 
struct Model:Observable 
{ 
   //data members title_caption, version_caption, credits_caption 
   string title_caption; 
   string version_caption; 
   string credits_caption; 
   
   //data members title, version, credits 
   string title; 
   string version; 
   string credits; 
   
   //constructor 
   Model() : 
    title_caption("Title: "), 
    version_caption("Version: "), 
    credits_caption("Credits: "), 
    title("Simple Model-View-Controller Implementation"), 
    version("0.2"), 
    credits("(put your name here)") 
    { } 
   
   //getCredits_Caption, getTitle_Caption, getVersion_Caption 
   string getCredits_Caption(){return credits_caption;} 
   string getTitle_Caption(){return title_caption;} 
   string getVersion_Caption(){return version_caption;} 
   
   //getCredits, getTitle, getVersion 
   string getCredits(){return credits;} 
   string getTitle(){return title;} 
   string getVersion(){return version;} 
   
   //setCredits, setTitle, setVersion 
   void setCredits(string a){credits=a;notifyObservers();} 
   void setTitle(string a){title=a;notifyObservers();} 
   void setVersion(string a){version=a;notifyObservers();} 
  /* 
   * AK notifyObservers(a) for credit, title and version. 
   * All as per discussion in View and Observer *  
   */ 
};


/* 
AK: 
Great stuff ;-) This satisfies a major principle of the MVC 
architecture, the separation of model and view.

The model now has NO View material in it, this model can now be used in 
other applications. 
You can use it with command line apps (batch, testing, reports, ...), 
web, gui, etc.

Mind you "MVC with Passive Model" is a variation of MVC where the model 
doesn't get even involved with the Observer pattern.

In that case the Controller would trigger a model update *and it* could 
also supply the latest info do the Views. This is a fairly common MVC 
variation, especially with we apps. 
*/

 

 //struct TitleView, specialized Observer 
struct TitleView:Observer 
{ 
/* 
 * AK: 
 * I like to get a reference to the model via a constructor to avoid 
 * a static_cast in update and to avoid creating zombie objects. 
 * 
 * A zombie object is instantiated but is unusable because it 
 * is missing vital elements. Dangerous. Getting model via the 
 * constructor solves this problem.

 Model model; 
 // Cons. 
 TitleView (Model* m) ....

RE-USABILITY. 
Some views are better off working with the full Model, yet others are 
better off being dumber.

I like to have two kinds of Views. Those that work with full Model (A) 
and those that only work with a limited more abstract data type (B).

Type A. 
Complex application specific views are better off getting the full 
model, they can then just pick and choose what they need from the full 
model without missing something all the time. Convenient.

Type B. 
These only require abstract or generic data types.

Consider a PieChartView, it doesn't really need to know about the full 
Model of a particular application, it can get by with just float 
*values[] or vector<float>;

By avoiding Model you can then reuse PieChartView in other applications 
with different models.

For this to be possible you must use the 2 argument version of 
notifyObservers. See comments on Observer class.

See my Java example NameView. That view only knows about a String, not 
the full Model. 
*/


   //update 
   void update(void*a) 
  /* 
   *AK:void update(void*a, void*arg) is often better. As per discussion 
  above. 
   */ 
   { 
   cout<<static_cast<Model*>(a)->getTitle_Caption(); 
   cout<<static_cast<Model*>(a)->getTitle(); 
   cout<<endl; 
   } 
}; 
 
 
 //struct VersionView, specialized Observer 
struct VersionView:Observer 
{ 
 
 //update 
 void update(void*a) 
 { 
 cout<<static_cast<Model*>(a)->getVersion_Caption(); 
 cout<<static_cast<Model*>(a)->getVersion(); 
 cout<<endl; 
 } 
}; 
 
 
 //struct CreditsView, specialized Observer 
struct CreditsView:Observer 
{ 
 
 //update 
 void update(void*a) 
 { 
 cout<<static_cast<Model*>(a)->getCredits_Caption(); 
 cout<<static_cast<Model*>(a)->getCredits(); 
 cout<<endl; 
 } 
}; 
 
 
 //struct Views, pack all Observers together in yet another Observer 
struct Views:Observer 
{ 
 //data members titleview, versionview, creditsview 
 TitleView titleview; 
 VersionView versionview; 
 CreditsView creditsview; 
/* 
 * AK: 
 * Views are often hierarchical and composed of other Views. See 
Composite pattern. 
 * vector<View*> views; 
 * 
 * Views often manage (create and own) a Controller. 
 * 
 * Views may include their own Controller code (Delegate). 
 * 
*/ 
 //setModel 
 void setModel(Observable&a) 
 { 
 a.addObserver(&titleview); 
 a.addObserver(&versionview); 
 a.addObserver(&creditsview); 
 a.addObserver(this); 
 } 
 
 //update 
 void update(void*a) 
 { 
 cout<<"_____________________________"; 
 cout<<"\nType t to edit Title, "; 
 cout<<"v to edit Version, "; 
 cout<<"c to edit Credits. "; 
 cout<<"Type q to quit./n>>"; 
 } 
}; 
 
 
 //struct Controller, wait for keystroke and change Model 
 struct Controller 
/* 
 * AK: Controller can also be an Observer. 
 * 
 * There is much to say about Controller but IMHO we should defer 
 * that to another version. 
 */ 
{ 
   //data member model 
   Model*model; 
   
   //setModel 
   void setModel(Model&a){model=&a;} 
   
   //MessageLoop 
   void MessageLoop() 
   { 
    char c=' '; 
    string s; 
    while(c!='q') 
    { 
      cin>>c; 
      cin.ignore(256,'\n'); 
      cin.clear(); 
      switch(c) 
      { 
       case 'c': 
       case 't': 
       case 'v': 
       getline(cin,s); 
       break; 
      } 
      switch(c) 
      { 
       case 'c':model->setCredits(s);break; 
       case 't':model->setTitle(s);break; 
       case 'v':model->setVersion(s);break; 
      } 
    } 
   } 
}; 
 
 
 //struct Application, get Model, Views and Controller together 
struct Application 
{ 
 
   //data member model 
   Model model; 
   
   //data member views 
   Views views; 
   
   //data member controller 
   Controller controller; 
   
   //constructor 
   Application() 
   { 
   views.setModel(model); 
   controller.setModel(model); 
   model.notifyObservers(); 
   } 
   
   //run 
   void run(){controller.MessageLoop();} 
}; 
 
 
 //main 
int main() 
{ 
  Application().run(); 
  return 0; 
}