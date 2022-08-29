function varargout = app_graph(varargin)
% APP_GRAPH MATLAB code for app_graph.fig
%      APP_GRAPH, by itself, creates a new APP_GRAPH or raises the existing
%      singleton*.
%
%      H = APP_GRAPH returns the handle to a new APP_GRAPH or the handle to
%      the existing singleton*.
%
%      APP_GRAPH('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in APP_GRAPH.M with the given input arguments.
%
%      APP_GRAPH('Property','Value',...) creates a new APP_GRAPH or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before app_graph_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to app_graph_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help app_graph

% Last Modified by GUIDE v2.5 27-Aug-2022 14:55:36

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @app_graph_OpeningFcn, ...
                   'gui_OutputFcn',  @app_graph_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before app_graph is made visible.
function app_graph_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to app_graph (see VARARGIN)

% Choose default command line output for app_graph
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes app_graph wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = app_graph_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in select_sensor.
function select_sensor_Callback(hObject, eventdata, handles)
% hObject    handle to select_sensor (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns select_sensor contents as cell array
%        contents{get(hObject,'Value')} returns selected item from select_sensor

selectedSensor = get(hObject,'Value')

switch selectedSensor
    case 2
        clear
        clc
        filename = 'acelData.txt';
        delimiterIn = ',';
        acel_data = importdata(filename,delimiterIn);
        acel_data_size = size(acel_data);
        acel_data_rows = acel_data_size(1);
        
        x_acel = acel_data(:, 1);
        y_acel = acel_data(:, 2);
        z_acel = acel_data(:, 3);
        
        t = [0:1:acel_data_rows-1];
        
        subplot(3,1,1);
        plot(t,x_acel, 'b', LineWidth=1);
        title('Eje X');
        grid on;
        
        subplot(3,1,2); 
        plot(t, y_acel, 'r', LineWidth=1);
        title('Eje Y');
        grid on;
        
        subplot(3,1,3)
        plot(t, z_acel, 'g', LineWidth=1);
        title('Eje Z');
        grid on;
    
    case 3
        clear
        clc
        filename = 'gyroData.txt';
        delimiterIn = ',';
        acel_data = importdata(filename,delimiterIn);
        acel_data_size = size(acel_data);
        acel_data_rows = acel_data_size(1);
        
        x_acel = acel_data(:, 1);
        y_acel = acel_data(:, 2);
        z_acel = acel_data(:, 3);
        
        t = [0:1:acel_data_rows-1];
        
        subplot(3,1,1);
        plot(t,x_acel, 'b', LineWidth=1);
        title('Eje X');
        grid on;
        
        subplot(3,1,2); 
        plot(t, y_acel, 'r', LineWidth=1);
        title('Eje Y');
        grid on;
        
        subplot(3,1,3)
        plot(t, z_acel, 'g', LineWidth=1);
        title('Eje Z');
        grid on;

    case 4
        clear
        clc
        filename = 'magnetData.txt';
        delimiterIn = ',';
        acel_data = importdata(filename,delimiterIn);
        acel_data_size = size(acel_data);
        acel_data_rows = acel_data_size(1);
        
        x_acel = acel_data(:, 1);
        y_acel = acel_data(:, 2);
        z_acel = acel_data(:, 3);
        
        t = [0:1:acel_data_rows-1];
        
        subplot(3,1,1);
        plot(t,x_acel, 'b', LineWidth=1);
        title('Eje X');
        grid on;
        
        subplot(3,1,2); 
        plot(t, y_acel, 'r', LineWidth=1);
        title('Eje Y');
        grid on;
        
        subplot(3,1,3)
        plot(t, z_acel, 'g', LineWidth=1);
        title('Eje Z');
        grid on;

    otherwise
end
% --- Executes during object creation, after setting all properties.
function select_sensor_CreateFcn(hObject, eventdata, handles)
% hObject    handle to select_sensor (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in graph_button.
function graph_button_Callback(hObject, eventdata, handles)
% hObject    handle to graph_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
