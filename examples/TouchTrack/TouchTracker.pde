// TOdo : make static ?

class TouchTracker {



  ////////////////////////////
  //    Settings
  //////////////////////////

  // todo : faire un flag pour savoir si le nombre est différent ? 
  // boolean keepA:ive = true;

  ArrayList<TouchPoint> touchPoints = new ArrayList<TouchPoint>();

  // Find Settings
  public  int closeThreshold = 20;



  // Remove the touch point after a certain delay
  public boolean removeAfterDelay = false;

  ////////////////////////////
  //    CONSTRUCTORS
  //////////////////////////


  public TouchTracker(int maxNumberOftouchPoints ) {
    for (int i =0; i < maxNumberOftouchPoints; i++) {
      touchPoints.add(new TouchPoint());
    }
  }



  ////////////////////////////
  //        SETTER
  //////////////////////////




  public void UpdateTouchPosition(PVector position, int weight) {
    // todo : loop here to see which one is the closest

    TouchPoint closestTouchPoint = null;
    float closestDist = 300;
    int closestVectorId = -1;
    for (int i = 0; i<touchPoints.size(); i++) {
      TouchPoint tp = touchPoints.get(i);

      float currentDist = PVector.dist(position, tp.position);
      if (currentDist < closestDist && currentDist < closeThreshold) {
        closestTouchPoint = tp;
      }
    }

    if (closestVectorId != -1) {
    }

    if (closestTouchPoint == null) {
      AssignNewTouchPoint(position, weight);
    } else {
      closestTouchPoint.Update(position, weight);
    }
  }


  void AssignNewTouchPoint(PVector position, int weight) {
    // todo : assigner le nouvel ID, a commecner par le preier
    // Looper et voir parmi lequel des points non actif est celui qui  été updaté le plus longtemps.
  }


  // override
  public void UpdateTouchPosition(PVector position) {
    UpdateTouchPosition(position, 1); // by default set touch wight of 1
  }




  public void update() {
    // here save time ?
    // On doit utiliser ça que si on fait du tracking en continu et qu'on utilise removeAfterDelay
    
    
  }


  public void EndUpdate() {

    println("Ici on doit cleaner et enlever les blobs qui n'ont pas été update cette frame");
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
    return touchPoints.get(0);
  }
}
