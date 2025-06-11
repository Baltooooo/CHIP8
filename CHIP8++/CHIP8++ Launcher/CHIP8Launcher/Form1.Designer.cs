namespace CHIP8Launcher
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selectROMToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cHIP8ModelToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cHIP8ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sUPERCHIPLegacyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sUPERCHIPModernToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.xOCHIPToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.editToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(800, 30);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.selectROMToolStripMenuItem});
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(46, 24);
            this.openToolStripMenuItem.Text = "File";
            // 
            // selectROMToolStripMenuItem
            // 
            this.selectROMToolStripMenuItem.Name = "selectROMToolStripMenuItem";
            this.selectROMToolStripMenuItem.Size = new System.Drawing.Size(169, 26);
            this.selectROMToolStripMenuItem.Text = "Select ROM";
            this.selectROMToolStripMenuItem.Click += new System.EventHandler(this.selectROMToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cHIP8ModelToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(76, 26);
            this.editToolStripMenuItem.Text = "Settings";
            // 
            // cHIP8ModelToolStripMenuItem
            // 
            this.cHIP8ModelToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cHIP8ToolStripMenuItem,
            this.sUPERCHIPLegacyToolStripMenuItem,
            this.sUPERCHIPModernToolStripMenuItem,
            this.xOCHIPToolStripMenuItem});
            this.cHIP8ModelToolStripMenuItem.Name = "cHIP8ModelToolStripMenuItem";
            this.cHIP8ModelToolStripMenuItem.Size = new System.Drawing.Size(224, 26);
            this.cHIP8ModelToolStripMenuItem.Text = "CHIP8 Model";
            // 
            // cHIP8ToolStripMenuItem
            // 
            this.cHIP8ToolStripMenuItem.Name = "cHIP8ToolStripMenuItem";
            this.cHIP8ToolStripMenuItem.Size = new System.Drawing.Size(233, 26);
            this.cHIP8ToolStripMenuItem.Text = "CHIP8";
            this.cHIP8ToolStripMenuItem.Click += new System.EventHandler(this.cHIP8ToolStripMenuItem_Click);
            // 
            // sUPERCHIPLegacyToolStripMenuItem
            // 
            this.sUPERCHIPLegacyToolStripMenuItem.Name = "sUPERCHIPLegacyToolStripMenuItem";
            this.sUPERCHIPLegacyToolStripMenuItem.Size = new System.Drawing.Size(233, 26);
            this.sUPERCHIPLegacyToolStripMenuItem.Text = "SUPERCHIP - Legacy";
            this.sUPERCHIPLegacyToolStripMenuItem.Click += new System.EventHandler(this.sUPERCHIPLegacyToolStripMenuItem_Click);
            // 
            // sUPERCHIPModernToolStripMenuItem
            // 
            this.sUPERCHIPModernToolStripMenuItem.Name = "sUPERCHIPModernToolStripMenuItem";
            this.sUPERCHIPModernToolStripMenuItem.Size = new System.Drawing.Size(233, 26);
            this.sUPERCHIPModernToolStripMenuItem.Text = "SUPERCHIP - Modern";
            this.sUPERCHIPModernToolStripMenuItem.Click += new System.EventHandler(this.sUPERCHIPModernToolStripMenuItem_Click);
            // 
            // xOCHIPToolStripMenuItem
            // 
            this.xOCHIPToolStripMenuItem.Name = "xOCHIPToolStripMenuItem";
            this.xOCHIPToolStripMenuItem.Size = new System.Drawing.Size(233, 26);
            this.xOCHIPToolStripMenuItem.Text = "XOCHIP";
            this.xOCHIPToolStripMenuItem.Click += new System.EventHandler(this.xOCHIPToolStripMenuItem_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem selectROMToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cHIP8ModelToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cHIP8ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sUPERCHIPLegacyToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sUPERCHIPModernToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem xOCHIPToolStripMenuItem;
    }
}

