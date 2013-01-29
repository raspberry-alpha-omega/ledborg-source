program ledborg_gui;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, runtimetypeinfocontrols, ledborg
  { you can add units after this };

{$R *.res}

begin
  Application.Title := 'LedBorg colour picker';
  Application.Initialize;
  Application.CreateForm(TfrmColours, frmColours);
  Application.Run;
end.

