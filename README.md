# Compressor

JUCEフレームワーク（`juce::dsp`）とC++で開発された、音楽制作ソフト（DAW）向けのコンプレッサー・オーディオプラグイン（AU / VST）です。
Logic ProなどのDAW上で動作し、4つの基本パラメータによるダイナミクス制御と、リサイズ可能なロータリーノブUIを備えています。

## 主な機能

* **リアルタイム・ダイナミクス処理 (`juce::dsp::Compressor<float>`)**
  * ステレオ入出力に対応したリアルタイム音声圧縮処理。
  * オーディオスレッドからロックフリー（`getRawParameterValue()->load()`）でパラメータを読み取り、低レイテンシでDSP処理を適用。
* **4つの基本コンプレッサー・パラメータ**
  * **Threshold**: `-60.0 dB` 〜 `0.0 dB`（初期値: `-12.0 dB`、刻み幅: `0.1`）
  * **Ratio**: `1.0 : 1` 〜 `20.0 : 1`（初期値: `4.0`、刻み幅: `0.1`）
  * **Attack**: `1.0 ms` 〜 `200.0 ms`（初期値: `20.0 ms`、刻み幅: `0.1`）
  * **Release**: `10.0 ms` 〜 `500.0 ms`（初期値: `100.0 ms`、刻み幅: `0.1`）
* **リサイズ対応カスタムUI (`CompresserPluginAudioProcessorEditor`)**
  * 4つのロータリーノブ（`RotaryVerticalDrag`）を横一列に均等配置し、下部に数値テキストボックスを表示。
  * ウィンドウの全辺ドラッグによるリサイズに対応（デフォルト: `600×350`、最小: `400×250`、最大: `1600×1200`）。
* **DAWセッション保存・復元（ステート管理）**
  * `AudioProcessorValueTreeState`（APVTS）とXMLシリアライズにより、DAWプロジェクト保存時のパラメータ状態の保持・復元に完全対応。

## システム構成・アーキテクチャ

| クラス / ファイル | 役割・概要 |
| :--- | :--- |
| **`CompresserPluginAudioProcessor`** (`PluginProcessor.h/.cpp`) | プラグインのコア処理クラス。ステレオバス設定、`AudioProcessorValueTreeState` によるパラメータ定義、`juce::dsp::Compressor` を使った `processBlock` 音声処理、XML形式での状態保存/復元を担当。 |
| **`CompresserPluginAudioProcessorEditor`** (`PluginEditor.h/.cpp`) | プラグインのGUIクラス。`SliderAttachment` によるUIとパラメータの双方向バインディング、`ComponentBoundsConstrainer` と `ResizableBorderComponent` によるウィンドウリサイズ制御を担当。 |

## ビルド環境・インストール手順

* **フレームワーク**: JUCE（`juce_dsp` モジュールを使用）
* **開発環境**: macOS / Xcode（`Builds/MacOSX` 内の `.xcodeproj` を使用）

```bash
# 1. リポジトリのクローン
git clone https://github.com
cd Compressor

# 2. Xcodeプロジェクトを開いてビルド
open Builds/MacOSX/*.xcodeproj
```

ビルド完了後、生成されたプラグインファイルをmacOSの所定ディレクトリへ配置（またはXcodeのビルド設定で自動コピー）すると、Logic Pro等のDAWから認識されます。
* **AU (Audio Units)**: `~/Library/Audio/Plug-Ins/Components/`
* **VST3**: `~/Library/Audio/Plug-Ins/VST3/`
