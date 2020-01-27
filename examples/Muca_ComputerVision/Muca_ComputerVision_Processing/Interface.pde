
import controlP5.*;

ControlP5 cp5;
Accordion accordion;

RadioButton r1,rthre;

void InterfaceSetup() {
  cp5 = new ControlP5(this);
  int x = 60;

  /*
   cp5.addSlider("thresholdBlob")
   .setPosition(destImg.width + 20 ,30)
   .setRange(0,255)
   ;
   */

  // group number 2, contains a radiobutton
  Group g0 = cp5.addGroup("Computer Vision")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(150)
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
    .activate(3)
    .moveTo(g0)
    ;

  for (Toggle t : r1.getItems()) {
    t.getCaptionLabel().setColorBackground(color(255, 0));
    t.getCaptionLabel().getStyle().moveMargin(-7, 0, 0, -3);
    t.getCaptionLabel().getStyle().movePadding(7, 0, 0, 3);
    t.getCaptionLabel().getStyle().backgroundWidth = 40;
    t.getCaptionLabel().getStyle().backgroundHeight = 13;
  }

  // create a toggle
  cp5.addToggle("enableThreshold")
    .setPosition(10, 60)
    .setSize(10, 10)
    .setLabel("Threshold")
    .setLabelVisible(false)
    .moveTo(g0)
    ;
    
  rthre = cp5.addRadioButton("Binary")
    .setPosition(10, 60)
    .setSize(10,10)
    .addItem("Binary", 1)
    .activate(1)
    .moveTo(g0)
    ;









  // group number 2, contains a radiobutton
  Group g1 = cp5.addGroup("Blob Detection")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(150)
    ;


  // create a toggle
  cp5.addToggle("enableBlobDetection")
    .setPosition(10, 10)
    .setSize(10, 10)
    .setLabel("Enable")
    .setLabelVisible(false)
    .moveTo(g1)
    ;

    cp5.addTextlabel("Enable")
    .setText("ENABLE")
    .setPosition(20,10).moveTo(g1);

  cp5.addSlider("thresholdMin")
    .setPosition(10, 60)
    .setRange(0, 255)
    .moveTo(g1)
    ;

  cp5.addSlider("thresholdMax")
    .setPosition(10, 70)
    .setRange(0, 255)
    .moveTo(g1)
    ;


  cp5.addSlider("thresholdblob")
    .setPosition(10, 80)
    .setRange(0, 1)
    .moveTo(g1)
    ;


  accordion = cp5.addAccordion("acc")
    .setPosition(destImg.width + x, 30)
    .setWidth(200)
    .addItem(g0)
    .addItem(g1)
    ;


  accordion.open(0, 1);
  accordion.setCollapseMode(Accordion.MULTI);
}



void controlEvent(ControlEvent theEvent) {
  if (theEvent.isFrom(r1)) {
    /*print("got an event from "+theEvent.getName()+"\t");
     for(int i=0;i<theEvent.getGroup().getArrayValue().length;i++) {
     print(int(theEvent.getGroup().getArrayValue()[i]));
     }
     println("\t "+theEvent.getValue());
     */
     if(int(theEvent.getGroup().getValue()) != -1)
    imgageProcessing = int(theEvent.getGroup().getValue());
  } else if(theEvent.isFrom(rthre)) {
    if(int(theEvent.getGroup().getValue()) == 1) enableThreshold = true; else  enableThreshold = false ;
  }
}
