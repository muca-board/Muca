enum TouchState {
  DOWN, 
    CONTACT, 
    UP, 
    ENDED
}

class TouchPoint {

  
    int fingerId = -1;
    
  TouchState state = TouchState.ENDED;
  PVector position = new PVector(0, 0);
  //PVector filteredPosition = new PVector(0,0);

  int weight = 0;


  int lastUpdate = 0;
  int deltaTimeUpdate = 0;

  public TouchPoint() {
    
    // assign a new finger ID 
    
  }
  
  
  public void NewId() {
   // todo : here assign new finger ID 
  }


  public boolean isActive() {
    return state == TouchState.CONTACT || state == TouchState.DOWN;
  }


  public void Update(PVector newPosition, int newWeight) {
    // calculate time
    int thisUpdate = millis();
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
  }


  public void DisableIfUpState() {
   if(state == TouchState.UP) {
     state = TouchState.ENDED;
   }
  }

  public void draw() {
    // here update position

    strokeWeight(1);
    if (state == TouchState.DOWN)   stroke(0, 255, 0);
    if (state == TouchState.CONTACT)   stroke(0, 0, 0);
    if (state == TouchState.UP) {
      println("DRAW UP"); //TODO : vérifier que ça dessine bien le up state
      stroke(255, 0, 0);
    }

    if (state != TouchState.ENDED) {
      text(fingerId, position.x, position.y);
      // fill(col);
      noFill();
      ellipse(position.x, position.y, weight*3, weight*3);

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
}
