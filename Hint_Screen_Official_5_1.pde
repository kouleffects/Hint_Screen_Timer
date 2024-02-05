import java.awt.datatransfer.*;
import java.awt.Toolkit;
import java.awt.datatransfer.StringSelection;

int totalTimeInSeconds = 3600;
int remainingTime;
boolean timerRunning = false;
boolean timerPaused = false;
int pauseStartTime = 0;

boolean enterPressed = false;
String userMessage = "";
String userMessageUpd = "";
boolean messageSent = false;

boolean displayFinalMessage = false;

void setup() {
  size(1920, 1080);

  String[] args = {"TwoFrameTest"};
  SecondApplet sa = new SecondApplet();
  PApplet.runSketch(args, sa);
}

void draw() {
  background(179, 131, 0);

  fill(87, 64, 0);
  rect(320, 150, 1280, 720);

  fill(0);
  textSize(130);
  text("Το Μυστικό της Πένας", 360, 120);

  fill(255);
  textSize(50);

  if (timerRunning && remainingTime > 0) {
    text("Ο χρόνος κυλάει:", 650, 250);
  } else if (timerRunning && remainingTime <= 0) {
    text("Τέλος χρόνου!", 650, 250);
    displayFinalMessage = true;
  } else if (messageSent) {
    text("Μήνυμα εστάλη στη δεύτερη οθόνη!", 650, 500);
  } else {
    text("Για να δούμε αν θα το λύσεις", 650, 250);
  }

  if (timerRunning && !timerPaused) {
    int elapsedTime = millis() / 1000;
    remainingTime = totalTimeInSeconds - elapsedTime;

    if (remainingTime >= 0) {
      displayTime(remainingTime, 900, 700);
    } else {
      displayTime(0, 900, 700);
      timerRunning = false;
    }
  }

  fill(255);
  textSize(40);
  text("Your message: " + userMessage, 420, 500);
}

void displayTime(int time, int x, int y) {
  int minutes = time / 60;
  int seconds = time % 60;

  String timeString = nf(minutes, 2) + ":" + nf(seconds, 2);
  fill(#FFB76E);
  textSize(200);
  text(timeString, 730, 430);
}

void keyTyped() {
  if (key == ' ' && !timerRunning) {
    timerRunning = true;
  } else if (key == 'p' || key == 'P') {
    togglePause();
  } else if (key == 't' || key == 'T') {
    modifyTotalTime();
  } else if (key == 'e' || key == 'E') {
    enterPressed = true;
  } else if ((key == ENTER) && userMessage.length() > 0) {
    userMessage += "\n";
  } else if (key == 'c' || key == 'C') {
    copyToClipboard(userMessage);
  } else if (key == 'v' || key == 'V') {
    String clipboardContent = getFromClipboard();
    if (clipboardContent != null) {
      userMessage += clipboardContent;
    }
  } else if (key == 'd' || key == 'D') {
    // Delete the whole text and start with a new one
    userMessage = "";
  } else if (timerRunning && !messageSent) {
    if (key == BACKSPACE && userMessage.length() > 0) {
      userMessage = userMessage.substring(0, userMessage.length() - 1);
    } else {
      userMessage += key;
    }
  }
}

void mousePressed() {
  if (!timerRunning) {
    timerRunning = true;
  } else {
    togglePause();
  }
}

void togglePause() {
  timerPaused = !timerPaused;

  if (timerPaused) {
    pauseStartTime = millis();
  } else {
    int elapsedTimePaused = (millis() - pauseStartTime) / 1000;
    totalTimeInSeconds += elapsedTimePaused;
  }
}

void modifyTotalTime() {
  String input = "";

  while (input.equals("")) {
    input = prompt("Enter the new total time in seconds:");
  }

  try {
    totalTimeInSeconds = Integer.parseInt(input) * 60;
  } catch (NumberFormatException e) {
    println("Invalid input. Please enter a valid number.");
  }
}

void copyToClipboard(String text) {
  StringSelection stringSelection = new StringSelection(text);
  Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
  clipboard.setContents(stringSelection, null);
}

String getFromClipboard() {
  Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
  Transferable contents = clipboard.getContents(null);

  if (contents != null && contents.isDataFlavorSupported(DataFlavor.stringFlavor)) {
    try {
      String pastedText = (String) contents.getTransferData(DataFlavor.stringFlavor);
      return compactText(pastedText, 50); // Adjust the maximum width as needed
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  return null;
}

String compactText(String text, int maxWidth) {
  String[] words = text.split("\\s+");
  StringBuilder compactedText = new StringBuilder();
  String currentLine = "";

  for (String word : words) {
    String testLine = currentLine + word + " ";
    float testWidth = textWidth(testLine);

    if (testWidth > 1040) {
      compactedText.append(currentLine.trim()).append("\n");
      currentLine = word + " ";
    } else {
      currentLine = testLine;
    }
  }

  compactedText.append(currentLine.trim());
  return compactedText.toString();
}

String prompt(String question) {
  return javax.swing.JOptionPane.showInputDialog(question);
}

public class SecondApplet extends PApplet {

  String receivedMessage = "";

  public void settings() {
    size(1920, 1080);
  }

  public void draw() {
    background(179, 131, 0);

    fill(87, 64, 0);
    rect(320, 150, 1280, 720);

    fill(0);
    textSize(130);
    text("Το Μυστικό της Πένας", 360, 120);

    fill(255);
    textSize(50);

    if (timerRunning && remainingTime > 0) {
      text("Ο χρόνος κυλάει:", 650, 250);
    } else if (displayFinalMessage) {
      fill(255, 0, 0);
      textSize(100);
      text("Τέλος χρόνου!", 630, 550);
    } else if (messageSent) {
      text("Μήνυμα εστάλη στη δεύτερη οθόνη!", 650, 500);
    } else {
      text("Για να δούμε αν θα το λύσεις", 650, 250);
    }

    if (enterPressed == true) {
      userMessageUpd = userMessage;
      enterPressed = false;
    }

    fill(255);
    textSize(40);
    text(" " + userMessageUpd, 420, 500);

    if (!receivedMessage.equals("")) {
      text(" ", 700, 300);
      text(receivedMessage, 650, 600);
    } else {
      text(" ", 650, 500);
    }

    if (timerRunning) {
      int elapsedTime = millis() / 1000;
      remainingTime = totalTimeInSeconds - elapsedTime;

      if (remainingTime >= 0) {
        displayTime(remainingTime, 900, 700);
      } else {
        displayTime(0, 900, 700);
      }
    }
  }

  void displayTime(int time, int x, int y) {
    int minutes = time / 60;
    int seconds = time % 60;

    String timeString = nf(minutes, 2) + ":" + nf(seconds, 2);
    fill(#FFB76E);
    textSize(200);
    text(timeString, 730, 430);
  }

  void receiveMessage(String message) {
    receivedMessage = message;
  }
}
