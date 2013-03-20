/*
 * Master.cpp
 */

#include "BaseFramework.hpp"

/*
  class Master : public Entity
 {
     bool loaded;
-    Dim windowDim;
+    WindowProperties &windowProps;

-    shared_ptr<Tex> brick;
-    PlaceholderEntityAABB *b;
+    unique_ptr<Entity> campaign;

 public:
     Master(WindowProperties &windowProperties) :
         loaded(false),
-        windowDim(windowProperties.dim),
-        b(nullptr)
+        windowProps(windowProperties)
     {
-        windowProperties.title = "Adventure";
+        windowProps.title = "Adventure";
+
+        unique_ptr<Entity> newCampaign(); // prototype
+        campaign = newCampaign();
     }
+
     void step() {
-        if( !loaded ) {
-            brick = Sys()->loadTexFromImage("brick.bmp");
-            b = new PlaceholderEntityAABB( Pt(50,50), Dim(10,10) );
-            loaded = true;
-        }
-        else {
-            Sys()->drawImage(*brick, Pt(0, 0), false);
-            Sys()->drawText("hello", Pt(0,0));
-
-            b->step();
-        }
+        campaign->step();
     }
+
     ~Master() {
-        delete b;
     }
 };



 */

class Master : public Entity
{
    bool loaded;
    WindowProperties &windowProps;

    unique_ptr<Entity> campaign;

public:
    Master(WindowProperties &windowProperties) :
        loaded(false),
        windowProps(windowProperties)
    {
        windowProps.title = "Adventure";

        unique_ptr<Entity> newCampaign(); // prototype
        campaign = newCampaign();
    }

    void step() {
        campaign->step();
    }

    ~Master() {
    }
};

Entity* getMaster(WindowProperties &windowProperties) {
    static Master *master = nullptr;
    if( master == nullptr )
        master = new Master(windowProperties);
    return master;
}
