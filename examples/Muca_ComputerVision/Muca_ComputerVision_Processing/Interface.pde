
import controlP5.*;

ControlP5 cp5;
Accordion accordion;

RadioButton r1, rthre, rBlob;

void InterfaceSetup() {
  cp5 = new ControlP5(this);
  int x = 60;


  ////////////////////////////////
  //      DEFAULT SETTINGS
  ///////////////////////////////
  Group g5 = cp5.addGroup("Grid settings")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(120)
    ;


  cp5.addSlider("thresholdMin")
    .setPosition(10, 10)
    .setRange(0, 255)
    .moveTo(g5)
    ;

  cp5.addSlider("thresholdMax")
    .setPosition(10, 25)
    .setRange(0, 255)
    .moveTo(g5)
    ;


  cp5.addSlider("gainValue")
    .setPosition(10, 40)
    .setRange(0, 31)
    .moveTo(g5)
    ;

  cp5.addButton("calib")
    .setValue(0)
    .setPosition(10, 55)
    .setSize(45, 19)
    .moveTo(g5)
    ;
  cp5.addButton("gain")
    .setValue(0)
    .setPosition(65, 55)
    .setSize(45, 19)
    .moveTo(g5)
    ;

  cp5.addSlider("filter")
    .setPosition(10, 80)
    .setRange(0, 5)
    .moveTo(g5)
    ; 

  cp5.addSlider("k")
    .setPosition(10, 95)
    .setRange(0, 1)
    .moveTo(g5)
    ;

  ////////////////////////////////
  //      COMPUTER VISION
  ///////////////////////////////

  Group g0 = cp5.addGroup("Computer Vision")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(110)
    ;

  r1 = cp5.addRadioButton("imgageProcessing") // INTER_NEAREST // 1 INTER_LINEAR  // 2 INTER_CUBIC  3 // INTER_AREA  4 // INTER_LANCZOS4
    .setPosition(10, 10)
    .setLabel("lol")
    .setSize(10, 10)
    .setColorForeground(color(120))
    .setColorLabel(color(255))
    .setItemsPerRow(1)
    .setSpacingColumn(50)
    .addItem("Nearest", 0)
    .addItem("Linear", 1)
    .addItem("Cubic", 2)
    .addItem("LANCZOS4", 4)
    .activate(0)
    .moveTo(g0)
    ;

  for (Toggle t : r1.getItems()) {
    t.getCaptionLabel().setColorBackground(color(255, 0));
    t.getCaptionLabel().getStyle().moveMargin(-7, 0, 0, -3);
    t.getCaptionLabel().getStyle().movePadding(7, 0, 0, 3);
    t.getCaptionLabel().getStyle().backgroundWidth = 40;
    t.getCaptionLabel().getStyle().backgroundHeight = 13;
  }


  rthre = cp5.addRadioButton("Binary")
    .setPosition(10, 70)
    .setSize(10, 10)
    .addItem("Binary", 1)
    .activate(1)
    .moveTo(g0)
    ;

  cp5.addSlider("thresholdBlobMin")
    .setPosition(10, 85)
    .setRange(0, 255)
    .moveTo(g0)
    ;


  ////////////////////////////////
  //      BLOB DETECTION
  ///////////////////////////////
  Group g1 = cp5.addGroup("Blob Detection")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(70)
    ;


  rBlob = cp5.addRadioButton("Enable Blob Detection")
    .setPosition(10, 10)
    .setSize(10, 10)
    .addItem("Enable Blob Detection", 1)
    //.activate(0)
    .moveTo(g1)
    ;







  ////////////////////////////////
  //      ACCORDION
  ///////////////////////////////


  accordion = cp5.addAccordion("acc")
    .setPosition(destImg.width + x, 30)
    .setWidth(200)
    .addItem(g5)
    .addItem(g0)
    .addItem(g1)
    ;


  accordion.open(0, 1);
  accordion.setCollapseMode(Accordion.MULTI);
}

public void calib() {
  String t= "c\n";
  println("Sending: " + t);
  skinPort.write(t);
}

public void gain() {
  String t= "g:"+gainValue+"\n";
  println("Sending: " + t);
  skinPort.write(t);
}


void controlEvent(ControlEvent theEvent) {
  if (theEvent.isFrom(r1)) {
    /*print("got an event from "+theEvent.getName()+"\t");
     for(int i=0;i<theEvent.getGroup().getArrayValue().length;i++) {
     print(int(theEvent.getGroup().getArrayValue()[i]));
     }
     println("\t "+theEvent.getValue());
     */
    if (int(theEvent.getGroup().getValue()) != -1)
      imgageProcessing = int(theEvent.getGroup().getValue());
  } else if (theEvent.isFrom(rthre)) {
    if (int(theEvent.getGroup().getValue()) == 1) enableThreshold = true; 
    else  enableThreshold = false ;
  } else if (theEvent.isFrom(rBlob)) {
    if (int(theEvent.getGroup().getValue()) == 1) enableBlobDetection = true;
    else enableBlobDetection = false;
  }
}
