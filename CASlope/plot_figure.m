function plot_figure
data=load('24.txt');
num=data(:,1);
length=size(data,2);
for ii=2:length
  plot(num,data(:,ii));  
  hold on;
end