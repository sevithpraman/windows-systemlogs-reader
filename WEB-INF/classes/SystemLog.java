public class SystemLog {
    int eventID;
    int eventType;
    String source;
    int category;
    int timeGenerated;
    String description;

    public SystemLog() {
    }

    public SystemLog(int eventID, int eventType, String source, int category, int timeGenerated, String description) {
        this.eventID = eventID;
        this.eventType = eventType;
        this.source = source;
        this.category = category;
        this.timeGenerated = timeGenerated;
        this.description = description;
    }

    public int getEventID() {
        return eventID;
    }

    public void setEventID(int eventID) {
        this.eventID = eventID;
    }

    public int getEventType() {
        return eventType;
    }

    public void setEventType(int eventType) {
        this.eventType = eventType;
    }

    public String getSource() {
        return source;
    }

    public void setSource(String source) {
        this.source = source;
    }

    public int getCategory() {
        return category;
    }

    public void setCategory(int category) {
        this.category = category;
    }

    public int gettimeGenerated() {
        return timeGenerated;
    }

    public void settimeGenerated(int timeGenerated) {
        this.timeGenerated = timeGenerated;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

}
