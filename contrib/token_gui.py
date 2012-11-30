#!/usr/bin/env python

import usb.core
import usb.util
import wx
import token_images
import pyunitoken as UT

ID_ICON_TIMER = wx.NewId()

def LoadImage(name):
    bmp = eval(name).Bitmap
    return bmp


class LoginDialog(wx.Dialog):
    def __init__(
            self, parent, ID, title, size=wx.DefaultSize, pos=wx.DefaultPosition, 
            style=wx.DEFAULT_DIALOG_STYLE,
            ):

        pre = wx.PreDialog()
        pre.SetExtraStyle(wx.DIALOG_EX_CONTEXTHELP)
        pre.Create(parent, ID, title, pos, size, style)

        self.PostCreate(pre)

        sizer = wx.BoxSizer(wx.VERTICAL)

        label = wx.StaticText(self, -1, "Enter token pin-code")
        label.SetHelpText("To access token data you need to enter pin-code")
        sizer.Add(label, 0, wx.ALIGN_CENTRE|wx.ALL, 5)

        box = wx.BoxSizer(wx.HORIZONTAL)

        label = wx.StaticText(self, -1, "Pin:")
        label.SetHelpText("Enter pin code")
        box.Add(label, 0, wx.ALIGN_CENTRE|wx.ALL, 5)

        self.text = wx.TextCtrl(self, -1, "", size=(80,-1), style = wx.TE_PASSWORD)
        self.text.SetHelpText("Enter pin here")
        box.Add(self.text, 1, wx.ALIGN_CENTRE|wx.ALL, 5)

        sizer.Add(box, 0, wx.GROW|wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5)

        box = wx.BoxSizer(wx.HORIZONTAL)


        line = wx.StaticLine(self, -1, size=(20,-1), style=wx.LI_HORIZONTAL)
        sizer.Add(line, 0, wx.GROW|wx.ALIGN_CENTER_VERTICAL|wx.RIGHT|wx.TOP, 5)

        btnsizer = wx.StdDialogButtonSizer()

        if wx.Platform != "__WXMSW__":
            btn = wx.ContextHelpButton(self)
            btnsizer.AddButton(btn)

        btn = wx.Button(self, wx.ID_OK)
        btn.SetHelpText("The OK button completes the dialog")
        btn.SetDefault()
        btnsizer.AddButton(btn)
        #self.Bind(wx.EVT_BUTTON, self.pincode, btn)

        btn = wx.Button(self, wx.ID_CANCEL)
        btn.SetHelpText("The Cancel button cancels the dialog. (Cool, huh?)")
        btnsizer.AddButton(btn)
        btnsizer.Realize()

        sizer.Add(btnsizer, 0, wx.ALIGN_CENTER_VERTICAL|wx.ALL, 5)

        self.SetSizer(sizer)
        sizer.Fit(self)

    def pincode(self, e):
        print self.text.GetValue()
        self.Destroy()

class TaskBarFrame(wx.Frame):
    def __init__(self, parent):
        wx.Frame.__init__(self, parent, style=wx.FRAME_NO_TASKBAR | wx.NO_FULL_REPAINT_ON_RESIZE)
        self.logged = False
        self.handle = 0
        self.tries = 0

        self.tbicon = wx.TaskBarIcon()
        self.tbicon.SetIcon(token_images.lock_icon.GetIcon(), '')
        self.Show(True)
#        self.Login()
        self.SetIconTimer()

    def Login(self):

        ret = False
        dlg = LoginDialog(self, -1, "Token login", size=(350, 200), style=wx.DEFAULT_DIALOG_STYLE,)
        dlg.CenterOnScreen()
        val = dlg.ShowModal()
        if val == wx.ID_OK:
            pin = dlg.text.GetValue()
            try:
                self.handle = UT.InitToken()
            except:
                wx.MessageBox("Token init failed")
            try:
                UT.TokenLogin(self.handle, UT.UT_USER_LEVEL_USER, pin)
                ret = True
            except IOError as e:
                print e.args
                if 'Password error' in e.args:
                    wx.MessageBox("Incorrect pin")
                    self.tries += 1
                ret = False
            return ret

    def SetIconTimer(self):
        self.icontimer = wx.Timer(self, ID_ICON_TIMER)
        wx.EVT_TIMER(self, ID_ICON_TIMER, self.BlinkIcon)
        self.icontimer.Start(500)

    def BlinkIcon(self, evt):
        dev = usb.core.find(idVendor=0x0403, idProduct=0xc585)
        if dev and not self.logged:
            self.logged = self.Login()
        elif dev is None and self.logged:
            self.logged = False
            self.handle = 0

        if dev is None and not self.logged:
            self.tbicon.SetIcon(token_images.lock_icon.GetIcon(), 'Red')
        else:
            self.tbicon.SetIcon(token_images.unlock_icon.GetIcon(), 'Black')


app = wx.App(False)
frame = TaskBarFrame(None)
frame.Show(False)
app.MainLoop()

