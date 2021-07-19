import { ComponentFixture, TestBed } from '@angular/core/testing';

import { LibJessicaComponent } from './lib-jessica.component';

describe('LibJessicaComponent', () => {
  let component: LibJessicaComponent;
  let fixture: ComponentFixture<LibJessicaComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [LibJessicaComponent],
    }).compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(LibJessicaComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
