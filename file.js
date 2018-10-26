$(document).ready(function(){
  $("#fileToUpload").on("change",function(e){
    var filename = e.target.value.split('\\').pop();
    $("#label_span").text(filename);
  });
});