unit ledborg;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Forms, Controls, Graphics, Dialogs, StdCtrls, Buttons, ExtCtrls, RTTICtrls;

type

  { TfrmColours }

  TfrmColours = class(TForm)
    butQuit: TButton;
    butDemo: TButton;
    grpSpeed: TGroupBox;
    grpColours: TGroupBox;
    lblColour000: TLabel;
    lblColour001: TLabel;
    lblColour002: TLabel;
    lblColour010: TLabel;
    lblColour011: TLabel;
    lblColour012: TLabel;
    lblColour020: TLabel;
    lblColour021: TLabel;
    lblColour022: TLabel;
    lblColour100: TLabel;
    lblColour101: TLabel;
    lblColour102: TLabel;
    lblColour110: TLabel;
    lblColour111: TLabel;
    lblColour112: TLabel;
    lblColour120: TLabel;
    lblColour121: TLabel;
    lblColour122: TLabel;
    lblColour200: TLabel;
    lblColour201: TLabel;
    lblColour202: TLabel;
    lblColour210: TLabel;
    lblColour211: TLabel;
    lblColour212: TLabel;
    lblColour220: TLabel;
    lblColour221: TLabel;
    lblColour222: TLabel;
    pnlDemo: TPanel;
    pnlColours: TPanel;
    pnlOther: TPanel;
    radColourHigh: TRadioButton;
    radColour8: TRadioButton;
    radColourCpu: TRadioButton;
    radColourPeakWave: TRadioButton;
    radColourMid: TRadioButton;
    radColourLow: TRadioButton;
    radColourRB: TRadioButton;
    radModerate: TRadioButton;
    radSlow: TRadioButton;
    radFast: TRadioButton;
    radColour27: TRadioButton;
    tmrDemo: TTimer;
    procedure butDemoClick(Sender: TObject);
    procedure butQuitClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure lblColour000Click(Sender: TObject);
    procedure lblColour001Click(Sender: TObject);
    procedure lblColour002Click(Sender: TObject);
    procedure lblColour010Click(Sender: TObject);
    procedure lblColour011Click(Sender: TObject);
    procedure lblColour012Click(Sender: TObject);
    procedure lblColour020Click(Sender: TObject);
    procedure lblColour021Click(Sender: TObject);
    procedure lblColour022Click(Sender: TObject);
    procedure lblColour100Click(Sender: TObject);
    procedure lblColour101Click(Sender: TObject);
    procedure lblColour102Click(Sender: TObject);
    procedure lblColour110Click(Sender: TObject);
    procedure lblColour111Click(Sender: TObject);
    procedure lblColour112Click(Sender: TObject);
    procedure lblColour120Click(Sender: TObject);
    procedure lblColour121Click(Sender: TObject);
    procedure lblColour122Click(Sender: TObject);
    procedure lblColour200Click(Sender: TObject);
    procedure lblColour201Click(Sender: TObject);
    procedure lblColour202Click(Sender: TObject);
    procedure lblColour210Click(Sender: TObject);
    procedure lblColour211Click(Sender: TObject);
    procedure lblColour212Click(Sender: TObject);
    procedure lblColour220Click(Sender: TObject);
    procedure lblColour221Click(Sender: TObject);
    procedure lblColour222Click(Sender: TObject);
    procedure radColourChange(Sender: TObject);
    procedure radSpeedChange(Sender: TObject);
    procedure tmrDemoTimer(Sender: TObject);
  private
    { private declarations }
    demoStage: integer;
    demoMode: integer;
    cpuUser: integer;
    cpuNice: integer;
    cpuSystem: integer;
    cpuIdle: integer;
    cpuIOWait: integer;
    cpuIrq: integer;
    cpuSoftIrq: integer;
    cpuRead: boolean;
    procedure SetConfig(config: string);
  public
    { public declarations }
  end; 

var
  frmColours: TfrmColours;

implementation

{$R *.lfm}

procedure TfrmColours.lblColour100Click(Sender: TObject);
begin
  SetConfig('100');
end;

procedure TfrmColours.lblColour101Click(Sender: TObject);
begin
  SetConfig('101');
end;

procedure TfrmColours.lblColour102Click(Sender: TObject);
begin
  SetConfig('102');
end;

procedure TfrmColours.lblColour110Click(Sender: TObject);
begin
  SetConfig('110');
end;

procedure TfrmColours.lblColour111Click(Sender: TObject);
begin
  SetConfig('111');
end;

procedure TfrmColours.lblColour112Click(Sender: TObject);
begin
  SetConfig('112');
end;

procedure TfrmColours.lblColour120Click(Sender: TObject);
begin
  SetConfig('120');
end;

procedure TfrmColours.lblColour121Click(Sender: TObject);
begin
  SetConfig('121');
end;

procedure TfrmColours.lblColour122Click(Sender: TObject);
begin
  SetConfig('122');
end;

procedure TfrmColours.lblColour010Click(Sender: TObject);
begin
  SetConfig('010');
end;

procedure TfrmColours.lblColour011Click(Sender: TObject);
begin
  SetConfig('011');
end;

procedure TfrmColours.lblColour012Click(Sender: TObject);
begin
  SetConfig('012');
end;

procedure TfrmColours.lblColour001Click(Sender: TObject);
begin
  SetConfig('001');
end;

procedure TfrmColours.lblColour000Click(Sender: TObject);
begin
  SetConfig('000');
end;

procedure TfrmColours.butQuitClick(Sender: TObject);
begin
  Halt();
end;

procedure TfrmColours.butDemoClick(Sender: TObject);
begin
  if tmrDemo.Enabled = False
  then begin
    pnlColours.Enabled := False;
    pnlColours.Visible := False;
    pnlDemo.Enabled := True;
    pnlDemo.Visible := True;
    tmrDemo.Enabled := True;
    butDemo.Caption := 'Normal Mode';
  end else begin
    tmrDemo.Enabled := False;
    pnlDemo.Enabled := False;
    pnlDemo.Visible := False;
    pnlColours.Enabled := True;
    pnlColours.Visible := True;
    butDemo.Caption := 'Demo Mode';
  end;
end;

procedure TfrmColours.FormCreate(Sender: TObject);
begin
  demoStage := 0;
  demoMode := 0;
end;

procedure TfrmColours.lblColour002Click(Sender: TObject);
begin
  SetConfig('002');
end;

procedure TfrmColours.lblColour020Click(Sender: TObject);
begin
  SetConfig('020');
end;

procedure TfrmColours.lblColour021Click(Sender: TObject);
begin
  SetConfig('021');
end;

procedure TfrmColours.lblColour022Click(Sender: TObject);
begin
  SetConfig('022');
end;

procedure TfrmColours.lblColour200Click(Sender: TObject);
begin
  SetConfig('200');
end;

procedure TfrmColours.lblColour201Click(Sender: TObject);
begin
  SetConfig('201');
end;

procedure TfrmColours.lblColour202Click(Sender: TObject);
begin
  SetConfig('202');
end;

procedure TfrmColours.lblColour210Click(Sender: TObject);
begin
  SetConfig('210');
end;

procedure TfrmColours.lblColour211Click(Sender: TObject);
begin
  SetConfig('211');
end;

procedure TfrmColours.lblColour212Click(Sender: TObject);
begin
  SetConfig('212');
end;

procedure TfrmColours.lblColour220Click(Sender: TObject);
begin
  SetConfig('220');
end;

procedure TfrmColours.lblColour221Click(Sender: TObject);
begin
  SetConfig('221');
end;

procedure TfrmColours.lblColour222Click(Sender: TObject);
begin
  SetConfig('222');
end;

procedure TfrmColours.radColourChange(Sender: TObject);
begin
  if radColour8.Checked then begin
    demoMode := 1;
  end else if radColourCpu.checked then begin
    cpuRead := False;
    demoMode := 2;
    SetConfig('012');
  end else if radColourHigh.checked then begin
    demoMode := 3;
  end else if radColourMid.checked then begin
    demoMode := 4;
  end else if radColourLow.checked then begin
    demoMode := 5;
  end else if radColourPeakWave.checked then begin
    demoMode := 6;
  end else if radColourRB.checked then begin
    demoMode := 7;
  end else begin
    demoMode := 0;
  end;
end;

procedure TfrmColours.radSpeedChange(Sender: TObject);
begin
  if radFast.Checked then begin
    tmrDemo.Interval := 100;
  end else if radModerate.checked then begin
    tmrDemo.Interval := 300;
  end else begin
    tmrDemo.Interval := 1000;
  end;
end;

procedure TfrmColours.tmrDemoTimer(Sender: TObject);
var
  fileIn: TextFile;
  cpuStats: string;
  cpuParts: TStringList;
  err: word;
  newUser: integer;
  newNice: integer;
  newSystem: integer;
  newIdle: integer;
  newIOWait: integer;
  newIrq: integer;
  newSoftIrq: integer;
  cpuDiff: integer;
  cpuBusy: integer;
begin
  case demoMode of
  0:   begin
    if demoStage > 26 then demoStage := 0;
    case demoStage of
    0:   SetConfig('100');
    1:   SetConfig('200');
    2:   SetConfig('211');
    3:   SetConfig('010');
    4:   SetConfig('020');
    5:   SetConfig('121');
    6:   SetConfig('001');
    7:   SetConfig('002');
    8:   SetConfig('112');
    9:   SetConfig('110');
    10:  SetConfig('220');
    11:  SetConfig('221');
    12:  SetConfig('011');
    13:  SetConfig('022');
    14:  SetConfig('122');
    15:  SetConfig('101');
    16:  SetConfig('202');
    17:  SetConfig('212');
    18:  SetConfig('000');
    19:  SetConfig('111');
    20:  SetConfig('222');
    21:  SetConfig('120');
    22:  SetConfig('021');
    23:  SetConfig('210');
    24:  SetConfig('012');
    25:  SetConfig('102');
    26:  SetConfig('201');
    otherwise begin
         SetConfig('100');
         demoStage := 0;
      end;
    end;
  end;
  1:    begin
    if demoStage > 7 then demoStage := 0;
    case demoStage of
    0:   SetConfig('200');
    1:   SetConfig('020');
    2:   SetConfig('002');
    3:   SetConfig('220');
    4:   SetConfig('022');
    5:   SetConfig('202');
    6:   SetConfig('222');
    7:   SetConfig('000');
    otherwise begin
         SetConfig('200');
         demoStage := 0;
      end;
    end;
  end;
  2:    begin
    AssignFile(fileIn, '/proc/stat');
    Reset(fileIn);
    ReadLn(fileIn, cpuStats);
    cpuParts := TStringList.Create;
    try
      cpuParts.Delimiter := ' ';
      cpuParts.DelimitedText := cpuStats;
      Val(cpuParts[1], newUser, err);
      Val(cpuParts[2], newNice, err);
      Val(cpuParts[3], newSystem, err);
      Val(cpuParts[4], newIdle, err);
      Val(cpuParts[5], newIOWait, err);
      Val(cpuParts[6], newIrq, err);
      Val(cpuParts[7], newSoftIrq, err);
    finally
      cpuParts.Free;
    end;
    if cpuRead then begin
      cpuBusy := (newUser - cpuUser) + (newNice - cpuNice) + (newSystem - cpuSystem) +
                 (newIrq - cpuIrq) + (newSoftIrq - cpuSoftIrq) + (newIOWait - cpuIOWait);
      cpuDiff := cpuBusy + (newIdle - cpuIdle);
    end;
    cpuUser := newUser;
    cpuNice := newNice;
    cpuSystem := newSystem;
    cpuIdle := newIdle;
    cpuIOWait := newIOWait;
    cpuIrq := newIrq;
    cpuSoftIrq := newSoftIrq;
    if cpuRead then begin
      demoStage := (cpuBusy * 5) div cpuDiff;
      if demoStage > 4 then demoStage := 0;
      case demoStage of
      0:   SetConfig('020');
      1:   SetConfig('120');
      2:   SetConfig('220');
      3:   SetConfig('210');
      4:   SetConfig('200');
      otherwise begin
          SetConfig('020');
          demoStage := 0;
        end;
      end;
    end else begin
      SetConfig('012');
      cpuRead := True;
    end;
    end;
  3:    begin
    if demoStage > 5 then demoStage := 0;
    case demoStage of
    0:   SetConfig('211');
    1:   SetConfig('221');
    2:   SetConfig('121');
    3:   SetConfig('122');
    4:   SetConfig('112');
    5:   SetConfig('212');
    otherwise begin
         SetConfig('211');
         demoStage := 0;
      end;
    end;
  end;
  4:    begin
    if demoStage > 11 then demoStage := 0;
    case demoStage of
    0:   SetConfig('200');
    1:   SetConfig('210');
    2:   SetConfig('220');
    3:   SetConfig('120');
    4:   SetConfig('020');
    5:   SetConfig('021');
    6:   SetConfig('022');
    7:   SetConfig('012');
    8:   SetConfig('002');
    9:   SetConfig('102');
    10:  SetConfig('202');
    11:  SetConfig('201');
    otherwise begin
         SetConfig('200');
         demoStage := 0;
      end;
    end;
  end;
  5:    begin
    if demoStage > 5 then demoStage := 0;
    case demoStage of
    0:   SetConfig('100');
    1:   SetConfig('110');
    2:   SetConfig('010');
    3:   SetConfig('011');
    4:   SetConfig('001');
    5:   SetConfig('101');
    otherwise begin
         SetConfig('100');
         demoStage := 0;
      end;
    end;
  end;
  6:   begin
    if demoStage > 29 then demoStage := 0;
    case demoStage of
    0:   SetConfig('100');
    1:   SetConfig('200');
    2:   SetConfig('211');
    3:   SetConfig('200');
    4:   SetConfig('100');
    5:   SetConfig('110');
    6:   SetConfig('220');
    7:   SetConfig('221');
    8:   SetConfig('220');
    9:   SetConfig('110');
    10:  SetConfig('010');
    11:  SetConfig('020');
    12:  SetConfig('121');
    13:  SetConfig('020');
    14:  SetConfig('010');
    15:  SetConfig('011');
    16:  SetConfig('022');
    17:  SetConfig('122');
    18:  SetConfig('022');
    19:  SetConfig('011');
    20:  SetConfig('001');
    21:  SetConfig('002');
    22:  SetConfig('112');
    23:  SetConfig('002');
    24:  SetConfig('001');
    25:  SetConfig('101');
    26:  SetConfig('202');
    27:  SetConfig('212');
    28:  SetConfig('202');
    29:  SetConfig('101');
    otherwise begin
         SetConfig('100');
         demoStage := 0;
      end;
    end;
  end;
  7:   begin
    if demoStage > 5 then demoStage := 0;
    case demoStage of
    0:   SetConfig('100');
    1:   SetConfig('200');
    2:   SetConfig('100');
    3:   SetConfig('001');
    4:   SetConfig('002');
    5:   SetConfig('001');
    otherwise begin
         SetConfig('100');
         demoStage := 0;
      end;
    end;
  end;
  end;

  demoStage := demoStage + 1;
end;

{ TfrmColours }
procedure TfrmColours.SetConfig(config: string);
begin
  with TStringList.Create do
  try
    Add(config);
    SaveToFile('/dev/ledborg');
    //SaveToFile('C:\\Temp\\ledborg');
  finally
    Free;
  end;
end;


end.

