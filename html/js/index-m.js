var vender = document.getElementById('ven');
var uecsid = document.getElementById('ueid');

// UECSIDフィールドの初期選択肢
var initialOptions = bField.innerHTML;

// Aフィールドの変更時に実行する関数
function changeOptions() {
  // Bフィールドの選択肢を初期状態に戻す
  bField.innerHTML = initialOptions;

  // Aフィールドの選択結果に応じてBフィールドの選択肢を変更
  var selectedOption = aField.value;
  if (selectedOption === '選択1') {
    bField.innerHTML = '<option value="選択1-1">選択1-1</option><option value="選択1-2">選択1-2</option>';
  } else if (selectedOption === '選択2') {
    bField.innerHTML = '<option value="選択2-1">選択2-1</option><option value="選択2-2">選択2-2</option>';
  }
}

// Aフィールドの変更イベントにchangeOptions関数を割り当て
aField.addEventListener('change', changeOptions);

