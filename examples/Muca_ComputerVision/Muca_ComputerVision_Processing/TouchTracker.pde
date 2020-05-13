// TOdo : make static ?

class TouchTracker {


  ////////////////////////////
  //    Settings
  //////////////////////////

  // todo : faire un flag pour savoir si le nombre est différent ? 
  // boolean keepA:ive = true;

  ArrayList<TouchPoint> touchPoints = new ArrayList<TouchPoint>();
  ArrayList<TouchPoint> touchPointsUpdated = new ArrayList<TouchPoint>();

  /*
  // Remove the touch point after a certain delay
   public boolean removeAfterDelay = false;
   
   boolean isUpdating = false;
   int startUpdateTime = 0;
   int endUpdateTime = 0;
   
   int  delayBeforeResetingTouchPoint = 150;
   */
  // Find Settings
  public  int closeThreshold = 30;






  int maxTouchPoints = 5; // default 5


  ////////////////////////////
  //    CONSTRUCTORS
  //////////////////////////

  public TouchTracker() {
  }

  public TouchTracker(int maxNumberOftouchPoints ) {
    maxTouchPoints = maxNumberOftouchPoints;
  }

  ////////////////////////////
  //        SETTER
  //////////////////////////


  //TODO : en fait il faudrait feeder tous les points et regarder pour chaque quel est le plus proche 

  public void UpdateTouchPosition(PVector position, int weight) {



    touchPointsUpdated.add(new TouchPoint(position, weight));
  }

  // override
  public void UpdateTouchPosition(PVector position) {
    UpdateTouchPosition(position, 1); // by default set touch wight of 1
  }



  TouchPoint CreateNewTouchPoint(PVector position, int weight) {
    // Find availableID
    int targetid = -1;


    for (int id = 0; targetid == -1 && id < maxTouchPoints; id++) {
      boolean isFree = true; 
      for (int i = 0; i<touchPoints.size(); i++) {
        if (touchPoints.get(i).touchID == id) isFree = false;
      }
      if (isFree) {
        targetid = id;
        continue;
      }
    }

    if (targetid == -1) {
      println("EEEEEEEEEEEEEEEEEEEEEEEEEEERRRORRRR there is no touchpoint available");
    }

    print("Creating id with ");
    println(targetid);
    //Create TouchPoint
    TouchPoint tp = new TouchPoint(targetid);
    tp.Update(position, weight);
    touchPoints.add(tp);
    return tp;
  }





  public void update() {
    // here save time ?
    // On doit utiliser ça que si on fait du tracking en continu et qu'on utilise removeAfterDelay

    //isUpdating = true;

    //EndTouchFlaggedUp();
    // EndTouchNotActiveSinceLongTime();

    // EndTouchFlaggedUp
  }
  /*
  public void EndTouchFlaggedUp() {
   for (int i = 0; i<touchPoints.size(); i++) {
   //  touchPoints.get(i).DisableIfUpState();
   }
   }
   */


  //REset active state
  public void BeginUpdate() {
    for (TouchPoint tp : touchPoints) {
      tp.hasBeenUpdated = false;
    } 

    touchPointsUpdated.clear(); //  Clear temp TouchPoints
  }

  public void EndUpdate() {

    // Update ALl points
    UpdateAllPoints(); 

    //Remove unused points
    for (int i = touchPoints.size()-1; i >= 0; i--) {
      TouchPoint tp = touchPoints.get(i);
      if (tp.CheckIfShouldDisable()) {
        touchPoints.remove(i);
        println("remove");
        print( i );
      }
    }
  }


  public void UpdateAllPoints() {
    int activeTouchPointsNum = touchPoints.size();
    int updatedTouchPointsNum = touchPointsUpdated.size();

    // CASE 1: There is no active point
    if (touchPoints.isEmpty()) {
      for (TouchPoint tp : touchPointsUpdated) {
        CreateNewTouchPoint(tp.position, tp.weight);
      }
    }

    // CASE 2: There is the same number of updated points than touch points
    // CASE 3: There is one point removed 
    else if (updatedTouchPointsNum <= activeTouchPointsNum) { 
      FindAndUpdateClosestTouchPoint(false);
    }

    // CASE 4: There is one new point, hence force Update
    else if (updatedTouchPointsNum > activeTouchPointsNum) { 
      FindAndUpdateClosestTouchPoint(true);
    } else {
      println("##############################################");
    }
  }


  TouchPoint FindAndUpdateClosestTouchPoint(boolean forceUpdate) {
    TouchPoint closestTouchpoint = null;

    //   updatedTable:
    //  |  idTouchUpdated   |  targetTouchPoint     | 
    //  |      1            |        5              |


    int updatedTable[] = new int[touchPointsUpdated.size()];

    for (int i = 0; i<touchPointsUpdated.size(); i++) {
      TouchPoint updatedTP = touchPointsUpdated.get(i);

      //TouchPoint closestUpdatedTouchPoint = null;
      //  float closestDist = 100;
      float record = 5000;
      int index = -1;
      // todo : usilier le flag hasBeenUpdated.
      for (int k = 0; k<touchPoints.size(); k++) {
        TouchPoint tp = touchPoints.get(k);

        float currentDist = PVector.dist(updatedTP.position, tp.position);
        if (forceUpdate) {
          if (currentDist < record && currentDist < closeThreshold ) {
            index = k;
            record = currentDist;
          }
        } else {
          if (currentDist < record) {
            index = k;
            record = currentDist;
          }
        }
      }
      updatedTable[i] = index;
    }

    // APply updated table
    for (int i = 0; i<updatedTable.length; i++) {
      if (updatedTable[i] == -1) {
        CreateNewTouchPoint(
          touchPointsUpdated.get(i).position, 
          touchPointsUpdated.get(i).weight
          );
      } else 
      touchPoints.get(updatedTable[i]).Update(
        touchPointsUpdated.get(i).position, 
        touchPointsUpdated.get(i).weight
        );
      print(i);
      print(":");
      print(updatedTable[i]);
      print("|");
    }
    println();
    return closestTouchpoint;
  }


  ////////////////////////////
  //    GETTER
  //////////////////////////


  public int touchCount() { // TODO : would be better as a getter 
    int numOfActiveTouches = 0;
    for (TouchPoint tp : touchPoints) {
      if (tp.isActive()) numOfActiveTouches++;
    }
    return numOfActiveTouches;
  }

  public TouchPoint GetTouch(int index) {
    // retoruner  filtrer en fonction de l'id

    int tmpLoop = 0;
    TouchPoint touchPoint = null;
    for (int i = 0; i<touchPoints.size(); i++) {
      TouchPoint tp = touchPoints.get(i);
      if (tp.isActive() && touchPoint == null) {
        if (tmpLoop == index) touchPoint = tp;
        else  tmpLoop++;
      }
    }

    if (touchPoint == null) println("no touchpoint found");

    return touchPoint;
  }
}

enum TouchState {
  DOWN, 
    CONTACT, 
    UP, 
    ENDED
}

class TouchPoint {


  color[] colors = 
    { 
    #42a5f5, 
    #4caf50, 
    #ffa000, 
    #673ab7, 
    #00FF9F, 
    #795548, 
    #bdbdbd, 
    #ff7043, 
    #7e57c2, 
    #26a69a, 
    #f06292, 
    #E500FF, 
    #0052FF, 
    #FF007C, 
    #1000FF, 
    #FFFFFF, 
  };
  TouchState state = TouchState.ENDED;
  PVector position = new PVector(0, 0);
  //PVector filteredPosition = new PVector(0,0);

  int weight = 0;

  int lastUpdate = 0;
  int deltaTimeUpdate = 0;

  boolean hasBeenUpdated = false;
  int touchID = -1;


  public TouchPoint(int index) {
    touchID = index;
    // assign a new finger ID
  }

  public TouchPoint(PVector newPosition, int newWeight) {
    position = newPosition;
    weight = newWeight;
  }

  public boolean isActive() {
    return state == TouchState.CONTACT || state == TouchState.DOWN;
  }


  public void Update(PVector newPosition, int newWeight) {

    // calculate time
    int thisUpdate = millis();
    if (state == TouchState.ENDED) lastUpdate = thisUpdate; // We reset the last updated flag  

    deltaTimeUpdate = thisUpdate - lastUpdate;
    lastUpdate = millis(); // OR Date d = new Date(); long current = d.getTime()/1000; 

    position = newPosition;
    weight = newWeight;

    // Updating state
    switch(state) {
    case DOWN:
      state = TouchState.CONTACT;
      break;
    case CONTACT:
      state = TouchState.CONTACT;
      break;
    case UP:
      state = TouchState.CONTACT;
      println("Hum, this should not be possible, or it means that the touch was supposed to be deleted. Maybe switch to state CONTACT?");
      break;
    case ENDED:
      state = TouchState.DOWN;
      break;
    }


    hasBeenUpdated = true;
  }


  public void ResetUpdateState() {
    /* if (isActive() && !hasBeenUpdated)
     state = TouchState.UP;
     
     hasBeenUpdated =false;*/
  }

  public boolean CheckIfShouldDisable() {
    if (!hasBeenUpdated) {
      if (state == TouchState.UP) {
        state = TouchState.ENDED;
        return true;
      } else {
        state = TouchState.UP;
        return false;
      }
    } 
    return false;
  }




  public void draw() {
    // here update position

    strokeWeight(1);
    if (state == TouchState.DOWN)   stroke(0, 255, 0);
    if (state == TouchState.CONTACT)   stroke(0, 0, 0);
    if (state == TouchState.UP) {
      stroke(255, 0, 0);
    }

    //if (state != TouchState.ENDED) {
    if (true) {
      fill(colors[touchID]);
      textSize(18);
      text(touchID, position.x+10, position.y+10);
      // fill(col);
      //  noFill();
      stroke(50, 50, 255);
      strokeWeight(0);
      ellipse(position.x, position.y, weight, weight);

      line(position.x, position.y, position.x-10, position.y);
      line(position.x, position.y, position.x+10, position.y);
      line(position.x, position.y, position.x, position.y-10);
      line(position.x, position.y, position.x, position.y+10);
    }

    /*
    // Remove at the end
     if (state == TouchState.UP) {
     state = TouchState.ENDED;
     }
     */
  }

  public void Reset() {
    state = TouchState.ENDED;
    position = new PVector(-100, -100);
    deltaTimeUpdate  =  0;
  }
}
