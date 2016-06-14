using System;
using System.IO;
using AdPoolService.Extensions;

namespace AdPoolService.Logging
{
    public sealed class FileLog : LogBase, IDisposable {
        private readonly Stream stream;
        private readonly TextWriter writer;

        private readonly object writeLock = new object();

        public FileLog(FileInfo file) {
            stream = file.Open(FileMode.Append, FileAccess.Write, FileShare.Read);
            writer = new StreamWriter(stream);
        }

        public void Dispose() {
            if (writer != null) {
                writer.Dispose();
            }
            if (stream != null) {
                stream.Dispose();
            }
        }

        private void WriteMessage(string message) {
            lock (writeLock) {
                writer.WriteLine(message);
                writer.Flush();
            }
        }

        protected override void WriteMessage(Severity severity, string message) {
            WriteMessage("[{0}]\t{1}".Fmt(severity.ToString(), message));
        }
    }
}