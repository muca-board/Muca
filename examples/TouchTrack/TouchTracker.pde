class TouchTracker {
  
  
  ArrayList<TouchPoint> touchPoints = new ArrayList<TouchPoint>();
  
  
  public TouchTracker(int maxNumberOftouchPoints ) {
    for(int i =0; i < maxNumberOftouchPoints; i++) {
      touchPoints.add(new TouchPoint());
    }
    
  }
  
  

  public void UpdateFinger(PVector coordinates) {
    
  }  
  
  
}
