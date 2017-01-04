# muscr 0.1.0

## Goal

* prototyping muscr file format for leadsheet

---

## Features

* leadsheet editing with Visual Studio Code.
    - IntelliSense
        - chord
        - section name
    - chord tool tip displaying the note names constituting the chord

* LeadsheetJS integration to display the muscr contents.


---

## muscr file format for leadsheet example

```
@title: A Sample Song
@author: ghjang

@scale: C Major
@pitchRange: 4
@clef: G

@timeSignature: 4 / 4
@bpm: 100

A :=
{
    C, D | E, (B, G) | A   | A, C
    C    | Em        | Am  | F
}

songForm := { A }
```

---

## muscr file format for leadsheet PEG definition

```
```
