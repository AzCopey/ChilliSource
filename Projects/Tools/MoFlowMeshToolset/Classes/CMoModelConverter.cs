﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.Windows.Threading;
using System.IO;

namespace MoFlowMeshToolset.Classes
{
    /// <summary>
    /// struct for storing all the options for converting to momodel
    /// </summary>
    public struct MoModelOptions
    {
        public bool mbHasTexture;
        public bool mbHasMaterial;
        public bool mbHasAnimationData;
        public bool mbVertexHasPosition;
        public bool mbVertexHasNormals;
        public bool mbVertexHasTextureCoordinates;
        public bool mbVertexHasVertexColours;
        public bool mbVertexHasWeights;
        public bool mbVertexHasJointIndices;
        public bool mbMirrorInXZPlane;
        public bool mbSwapYAndZ;
        public bool mbFlipVerticalTexCoords;
        public bool mbCombineMeshes;
        public float mfScale;
    }

    public class CMoModelConverter
    {
        //constants
        const String kstrPathToMoModelTool = "ColladaToMoModelConverter.jar";
        const String kstrPathOutput = "Output/Models/";

        //private data
        MainWindow mMainWindow;
        bool mbTaskActive;
        MoModelOptions mOptions;
        string[] mastrFilenames;
        string mstrCurrentStandardError;

        /// <summary>
        /// Constructor
        /// </summary>
        public CMoModelConverter(MainWindow inParent)
        {
            mMainWindow = inParent;
            mbTaskActive = false;
        }

        /// <summary>
        /// Adds a line of text to the output window. This can be called
        /// from any thread.
        /// </summary>
        /// <param name="instrText"></param>
        public void WriteLineToOutput(string instrText)
        {
            mMainWindow.GetMainThreadDispatcher().BeginInvoke(new MainWindow.AddTextDelegate(mMainWindow.WriteLineToOutput), instrText);
        }

        /// <summary>
        /// Event called when there is new data from the processes std out
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ReadStandardOutputEvent(object sender, DataReceivedEventArgs e)
        {
            WriteLineToOutput(e.Data);
        }

        /// <summary>
        /// event received when there is new data from the process std error
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void ReadStandardErrorEvent(object sender, DataReceivedEventArgs e)
        {
            mstrCurrentStandardError += e.Data + "\n";
        }

        /// <summary>
        /// Takes each of the passed in files and runs them through the ColladaToMoStaticConverter, with the user selected options.
        /// </summary>
        /// <param name="inastrFilenames"></param>
        public void ConvertToMoModel(string[] inastrFilenames, MoModelOptions inOptions)
        {
            if (mbTaskActive == false)
            {
                //start progress bar
                mMainWindow.StartProgressBar((double)inastrFilenames.Length);

                //clear the current output window
                mMainWindow.ClearOutputWindow();

                //take a copy of the data to be used for conversion
                mOptions = inOptions;
                mastrFilenames = new string[inastrFilenames.Length];
                inastrFilenames.CopyTo(mastrFilenames, 0);

                //start the task and flag the background task as running.
                mbTaskActive = true;
                ThreadPool.QueueUserWorkItem(this.MoModelConversionTask);
            }
        }

        /// <summary>
        /// Task for loading a task in the background.
        /// </summary>
        public void MoModelConversionTask(object o)
        {
            List<KeyValuePair<string, string>> ErrorLog = new List<KeyValuePair<string, string>>();

            //iterate through each file
            foreach (string strFilepath in mastrFilenames)
            {
                WriteLineToOutput("Converting '" + mastrFilenames[0] + "':");

                //get the output name
                string[] strFilepathBrokenUp = strFilepath.Split('/');
                string strWrongExtension = strFilepathBrokenUp[strFilepathBrokenUp.Length - 1];
                string strOutputName = strWrongExtension.Replace(".dae", "");
                strOutputName = strOutputName.Replace(".DAE", "");

                //Create the output directory
                Directory.CreateDirectory(kstrPathOutput + strOutputName + "/");

                //create and start the process
                Process process = new Process();
                process.StartInfo.CreateNoWindow = true;
                process.StartInfo.FileName = "java.exe";
                process.StartInfo.Arguments = "-jar " + kstrPathToMoModelTool
                    + " --input \"" + strFilepath + "\""
                    + " --output \"" + kstrPathOutput + strOutputName + "/" + strOutputName + ".momodel\""
                    + " --features " + GetFeaturesString(mOptions)
                    + " --vertexdeclaration " + GetVertexDeclarationString(mOptions)
                    + " --transforms " + GetTransformsString(mOptions)
                    + " --logginglevel verbose"
                    + " --errordisplay whenrecieved";

                process.StartInfo.UseShellExecute = false;
                process.StartInfo.RedirectStandardOutput = true;
                process.StartInfo.RedirectStandardError = true;
                process.StartInfo.ErrorDialog = true;
                process.Start();

                //start asyc read all standard output and print it to the output window.
                mstrCurrentStandardError = "";
                process.OutputDataReceived += this.ReadStandardOutputEvent;
                process.ErrorDataReceived += this.ReadStandardErrorEvent;
                process.BeginErrorReadLine();
                process.BeginOutputReadLine();

                //wait for this process to finish
                process.WaitForExit();

                //remove the events
                process.OutputDataReceived -= this.ReadStandardOutputEvent;
                process.ErrorDataReceived -= this.ReadStandardErrorEvent;

                //store error output
                ErrorLog.Add(new KeyValuePair<string, string>(strOutputName, mstrCurrentStandardError));
                mstrCurrentStandardError = "";

                //incriment the progress bar
                mMainWindow.GetMainThreadDispatcher().BeginInvoke(new MainWindow.ProgressDelegate(mMainWindow.IncrimentProgressBar));
            }

            bool bErrorFound = false;
            //write the error output
            foreach (KeyValuePair<string, string> kvp in ErrorLog)
            {
                if (kvp.Value.Length > 2)
                {
                    WriteLineToOutput(kvp.Key + ":\n" + kvp.Value);
                    //WriteLineToOutput();
                    bErrorFound = true;
                }
            }

            if (bErrorFound == false)
            {
                WriteLineToOutput("All Files successfully converted!");
            }

            //finish the progress bar
            mMainWindow.GetMainThreadDispatcher().BeginInvoke(new MainWindow.ProgressDelegate(mMainWindow.EndProgressBar));

            //clean up and reset to allow the task to be started again.
            mastrFilenames = null;
            mbTaskActive = false;
        }

        /// <summary>
        /// Returns the "features" string  for passing to the MoModel converter.
        /// </summary>
        /// <param name="inOptions"></param>
        /// <returns></returns>
        private string GetFeaturesString(MoModelOptions inOptions)
        {
            string output = "";

            if (inOptions.mbHasTexture == true)
                output += "t";
            if (inOptions.mbHasMaterial == true)
                output += "m";
            if (inOptions.mbHasAnimationData == true)
                output += "a";

            if (output.Length == 0)
                output = "0";

            return output;
        }

        /// <summary>
        /// Returns the "vertex declaration" string for passing to the MoModel converter.
        /// </summary>
        /// <param name="inOptions"></param>
        /// <returns></returns>
        private string GetVertexDeclarationString(MoModelOptions inOptions)
        {
            string output = "";

            if (inOptions.mbVertexHasPosition == true)
                output += "p";
            if (inOptions.mbVertexHasTextureCoordinates == true)
                output += "t";
            if (inOptions.mbVertexHasNormals == true)
                output += "n";
            if (inOptions.mbVertexHasVertexColours == true)
                output += "c";
            if (inOptions.mbVertexHasWeights == true)
                output += "w";
            if (inOptions.mbVertexHasJointIndices == true)
                output += "j";

            if (output.Length == 0)
                output = "0";

            return output;
        }

        /// <summary>
        /// Returns the "transforms" string for passing to the MoModel converter.
        /// </summary>
        /// <param name="inOptions"></param>
        /// <returns></returns>
        private string GetTransformsString(MoModelOptions inOptions)
        {
            string output = "";

            if (inOptions.mbMirrorInXZPlane == true)
                output += "m";
            if (inOptions.mbSwapYAndZ == true)
                output += "y";
            if (inOptions.mbFlipVerticalTexCoords == true)
                output += "f";
            if (inOptions.mbCombineMeshes == true)
                output += "c";

            if (output.Length == 0)
                output = "0";

            return output;
        }
    }
}
