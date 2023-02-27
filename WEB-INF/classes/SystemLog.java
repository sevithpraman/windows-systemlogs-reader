public class SystemLog {
    String source;
    String description;

    public SystemLog() {
    }

    public SystemLog(String source, String description) {
        this.source = source;
        this.description = description;
    }

    public String getSource() {
        return source;
    }

    public void setSource(String source) {
        this.source = source;
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

}
