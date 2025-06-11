using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace CHIP8Launcher
{
    public partial class Form1 : Form
    {

        public string configPath = "CHIP8-config.txt";
        public string[] lines;
        public Form1()
        {
            InitializeComponent();
            lines = File.ReadAllLines(configPath);
            for (int i = 0; i < lines.Length; i++)
            {
                switch (lines[i])
                {
                    case "model=chip8":
                        cHIP8ToolStripMenuItem_Click(this, new EventArgs());
                        break;
                    case "model=superLegacy":
                        sUPERCHIPLegacyToolStripMenuItem_Click(this, new EventArgs());
                        break;
                    case "model=superModern":
                        sUPERCHIPModernToolStripMenuItem_Click(this, new EventArgs());
                        break;
                    case "model=xochip":
                        xOCHIPToolStripMenuItem_Click(this, new EventArgs());
                        break;

                }
            }
            this.Text = "CHIP8 Launcher";
            this.Icon = new Icon("CH8.ico");
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        void assignConfiguration(string Property, string value)
        {
            for (int i = 0; i < lines.Length; i++)
            {
                if (lines[i].StartsWith(Property))
                    lines[i] = Property + value;
            }
            File.WriteAllLines(configPath, lines);
        }

        private void selectROMToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string romPath;
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.InitialDirectory = "C:\\";
            ofd.Multiselect = false;
            ofd.Filter = "CHIP8 ROMS(*.ch8)|*.ch8";
            ofd.Title = "Select ROM";

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                romPath = ofd.FileName;
                assignConfiguration("romPath=", romPath);
                System.Diagnostics.Process.Start("CHIP8++.exe");
                this.Close();
            }


        }

        public void cHIP8ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            cHIP8ToolStripMenuItem.Checked = true;
            sUPERCHIPLegacyToolStripMenuItem.Checked = false;
            sUPERCHIPModernToolStripMenuItem.Checked = false;
            xOCHIPToolStripMenuItem.Checked = false;
            assignConfiguration("model=", "chip8");
        }

        public void sUPERCHIPLegacyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            cHIP8ToolStripMenuItem.Checked = false;
            sUPERCHIPLegacyToolStripMenuItem.Checked = true;
            sUPERCHIPModernToolStripMenuItem.Checked = false;
            xOCHIPToolStripMenuItem.Checked = false;
            assignConfiguration("model=", "superLegacy");
        }

        public void sUPERCHIPModernToolStripMenuItem_Click(object sender, EventArgs e)
        {
            cHIP8ToolStripMenuItem.Checked = false;
            sUPERCHIPLegacyToolStripMenuItem.Checked = false;
            sUPERCHIPModernToolStripMenuItem.Checked = true;
            xOCHIPToolStripMenuItem.Checked = false;
            assignConfiguration("model=", "superModern");
        }

        public void xOCHIPToolStripMenuItem_Click(object sender, EventArgs e)
        {
            cHIP8ToolStripMenuItem.Checked = false;
            sUPERCHIPLegacyToolStripMenuItem.Checked = false;
            sUPERCHIPModernToolStripMenuItem.Checked = false;
            xOCHIPToolStripMenuItem.Checked = true;
            assignConfiguration("model=", "xochip");
        }
    }
}
