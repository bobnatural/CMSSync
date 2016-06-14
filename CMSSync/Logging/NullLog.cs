namespace AdPoolService.Logging
{
    public class NullLog : LogBase {
        protected override void WriteMessage(Severity severity, string message) {
            // do nothing here
        }
    }
}