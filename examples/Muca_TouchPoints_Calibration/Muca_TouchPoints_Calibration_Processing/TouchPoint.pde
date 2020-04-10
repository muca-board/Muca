enum TouchState {
  DOWN, 
    CONTACT, 
    UP, 
    ENDED
}

class TouchPoint {


  TouchState currentState = TouchState.ENDED;

  // Flag is working only for DOWN and CONTACT // int flag = 3; // 0: DOWN_EVENT, 1:UP_EVENT, 2: CONTACT_EVENT, 3: NO_EVENT
  int hardwareId = -1;
  PVector calculatedPosition = new PVector(0, 0);
  PVector receivedPosition = new PVector(0, 0);
  int weight =0;
  color col;

  public TouchPoint(int newId) {
    hardwareId = newId;

    switch(hardwareId) {
    case 0:
      col = color(250, 0, 0);
      break;
    case 1:
      col = color(0, 255, 0);
      break;
    case 2:
      col = color(0, 0, 255);
      break;
    case 3:
      col = color(250, 0, 255);
      break;
    case 4:
      col = color(0, 255, 255);
      break;
    }
  }



  public void Update(int newX, int newY, int newWeight) {
    receivedPosition = new PVector(newX, newY);
    weight = newWeight;

    // Updating states
    if (currentState == TouchState.ENDED) {
      currentState = TouchState.DOWN;
    } else if (currentState == TouchState.DOWN) {
      currentState = TouchState.CONTACT;
    }
  }

  public  void Draw() {
    UpdatePosition();


    strokeWeight(1);
    if (currentState == TouchState.DOWN)   stroke(0, 255, 0);
    if (currentState == TouchState.CONTACT)   stroke(0, 0, 0);
    if (currentState == TouchState.UP) {
      stroke(255, 0, 0);
    }

    if (currentState != TouchState.ENDED) {
     // fill(col);
      noFill();
      ellipse(calculatedPosition.x, calculatedPosition.y, weight*3, weight*3);
      
      line(calculatedPosition.x,calculatedPosition.y,calculatedPosition.x-10,calculatedPosition.y);
      line(calculatedPosition.x,calculatedPosition.y,calculatedPosition.x+10,calculatedPosition.y);
      line(calculatedPosition.x,calculatedPosition.y,calculatedPosition.x,calculatedPosition.y-10);
      line(calculatedPosition.x,calculatedPosition.y,calculatedPosition.x,calculatedPosition.y+10);
    }

    // Remove at the end
    if (currentState == TouchState.UP)  {
      currentState = TouchState.ENDED;
    }
  }


  //TODO: Lerp position for a smoother result
  // The event touch up is 
  void UpdatePosition() {
    if(currentState == TouchState.DOWN) calculatedPosition = receivedPosition; // for the first time it hits
    calculatedPosition = calculatedPosition.lerp(receivedPosition, 0.5);
  }

  public boolean isActive() {
    return currentState == TouchState.DOWN || currentState == TouchState.CONTACT;
  }


  public void Remove() {
    if (currentState == TouchState.DOWN || currentState == TouchState.CONTACT) {
      currentState = TouchState.UP;
    }
  }

  //  0:2:558:157:16|1:2:396:329:13|3:2:391:562:20|2:2:596:317:23|
}
